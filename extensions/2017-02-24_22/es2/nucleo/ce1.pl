use JSON::XS;
use Data::Dumper;
use IO::Handle;

($dev = $0) =~ s/\.pl$//;

open $in, "<", "$dev.out";
open $out, ">", "$dev.in";
open $ain, "<", "${dev}a.out";
open $aout, ">", "${dev}a.in";

$out->autoflush(1);
$aout->autoflush(1);

$json = JSON::XS->new;

#$debug = 1;

$config = {
		bars => [
			{
				name => 'ce-io',
				type => 'io',
				size => 32,
			}
		],
		'interrupt-pin' => 2
	};


my $VPTRHI;
my $VPTRLO;
my $CNT;
my $CMD;
my $STS;
my $pend_intr;

sub dbg {
	my $msg = shift;
	if ($debug) {
		printf "$0: $msg", @_;
	}
}

sub send {
	my ($ch, $msg) = @_;
	my $r = $json->encode($msg);
	&dbg("### $0: sending '$r' on $ch\n");
	if ($ch eq "sync") {
		print $out "$r\n";
	} else {
		print $aout "$r\n";
	}
	&dbg("### $0: done sending\n");
}

sub receive {
	my $ch = shift || "sync";
	my $i = \*$in;
	if ($ch eq "async") {
		$i = \*$ain;
	}
	&dbg("### $0: receiving from $ch\n");
	while (<$i>) {
	       	if ($obj = $json->incr_parse($_)) {
			&dbg("<-" . Dumper($obj));
			return $obj;
		}
	}
	exit;
}


sub send_ret {
	my $arg = shift || 0;
	&send("sync", { return => $arg});
}

sub send_intr {
	if (!$pend_intr) {
		&send("async", { "raise-irq" => 1 });
		&receive("async");
		$pend_intr = 1;
	}
}

sub clear_intr {
	if ($pend_intr) {
		&send("async", { "lower-irq" => 1 });
		&receive("async");
		$pend_intr = 0;
	}
}

my $next_len = 0;

sub get_des
{
	my $p = shift;
	&send("async", { "dma-read" => { addr => $p, len => 8 } });
	my $obj = &receive("async");
	my @a = ();
	if (!(@a = @{$obj->{'bytes'}})) {
		&dbg("no bytes?\n");
		return 0;
	}
	my $addr = 0;
	$a[6] &= 0x0f;
	$a[1] &= 0xf0;
	for (my $i = 6; $i >= 1; $i--) {
		$addr <<= 8;
		$addr |= $a[$i];
	}
	return (int($a[0]), $addr << 8);	
}

sub translate {
	my $v = int(shift);
	my $sh = 36;
	my $b = int($CR3), $d;
	for (my $i = 4; $i >= 1; $i--) {
		($d, $b) = &get_des($b | (($v >> $sh) & 07770));
		&dbg("translate(%x) lev $i -> access %x, next %x\n", $v, $d, $b);
		if (!($d & 1)) {
			return 0;
		}
		$sh -= 9;
	}
	return $b | ($v & 0xfff);
}

my $chr = 0;
sub start_dma {
	my $v = $VPTRHI << 32 | $VPTRLO;
	&dbg("### DMA %x+%x CR3=%x\n", $v, $CNT, $CR3);
	while ($CNT) {
		my $p = &translate($v);
		if (!$p) {
			$STS |= 2;
			last;
		}
		my $next_pag = ($p + 4096) & ~0xfff;
		$len = $next_pag - $p;
		$trans = ($len, $CNT)[$CNT < $len];
		$CNT -= $trans;
		#dbg("### addr %x - %x len $len trans $trans remain $CNT\n", $v, $p);
		my $stop = $chr+$trans-1;
		$reply = {
			"dma-write" => {
				addr => int($p),
				len  => int($trans),
				bytes => [map { ($_ % 26) + 65 } $chr..$stop],
			}
		};
		$chr = $stop;
		&send("async", $reply);
		&receive("async");
		$v += $trans;
	}
	&send_intr;
}

while (1) {
	$obj = &receive;
	if ($$obj{'get-config'}) {
		&send("sync", $config);
		&send_ret;
	} elsif ($i = $$obj{'write'}) {
		$a = $i->{'addr'};
		$s = $i->{'size'};
		$v = $i->{'val'};
		&send_ret if  ($s != 4);
		&dbg("### $0: writing $v at $a\n");
		my $dma = 0;
		if ($a == 0) {
			$VPTRHI = $v;
		} elsif ($a == 4) {
			$VPTRLO = $v;
		} elsif ($a == 8) {
			$CNT = $v;
		} elsif ($a == 12) {
			$CR3 = $v;
		} elsif ($a == 16) {
			$STS = $v;
		} elsif ($a == 20) {
			$CMD = $v;
			if ($CMD == 1) {
				$dma = 1;
			}
		}
		&send_ret;
		if ($dma) {
			&start_dma;
		}
	} elsif ($i = $$obj{'read'}) {
		$a = $i->{'addr'};
		&send_ret if  ($a % 4 || $a >= 24);
		$s = $i->{'size'};
		&send_ret if  ($s != 4);
		&dbg("### $0: reading from $a\n");
		if ($a == 0) {
			&send_ret($VPTRHI);
		} elsif ($a == 4) {
			&send_ret($VPTRLO);
		} elsif ($a == 8) {
			&send_ret($CNT);
		} elsif ($a == 12) {
			&send_ret($CR3);
		} elsif ($a == 16) {
			&send_ret($STS);
			&clear_intr;
		} elsif ($a == 20) {
			&send_ret($CMD);
		}
	}			
}
