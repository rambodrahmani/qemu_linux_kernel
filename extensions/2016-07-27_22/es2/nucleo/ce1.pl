use JSON::XS;
use Data::Dumper;
use IO::Handle;

($dev = $0) =~ s/\.pl$//;

open $in, "<", "$dev.out";
open $out, ">", "$dev.in";

$out->autoflush(1);

$json = JSON::XS->new;

#$debug = 1;

$config = {
		bars => [
			{
				name => 'ce-io',
				type => 'io',
				size => 16,
			}
		],
		'interrupt-pin' => 2
	};


my $BMPTR;
my $CMD;
my $STS;
my $pend_intr;

sub dbg {
	my $msg = shift;
	print "$0: $msg" if $debug;
}

sub send {
	my ($ch, $msg) = @_;
	my $r = $json->encode($msg);
	&dbg("### $0: sending '$r' on $ch\n");
	if ($ch eq "sync") {
		print $out "$r\n";
	} else {
		print $async "$r\n";
	}
}

sub receive {
	while (<$in>) {
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
		&send("sync", { "raise-irq" => 1 });
		&receive;
		$pend_intr = 1;
	}
}

sub clear_intr {
	if ($pend_intr) {
		&send("sync", { "lower-irq" => 1 });
		&receive;
		$pend_intr = 0;
	}
}

my @lenghts = (6000, 50, 8129, 4095);
my $next_len = 0;

my $chr = 0;
sub start_dma {
	my $des = int($BMPTR);
	my $cur_len = $lenghts[$next_len++ % @lenghts];
	do {
		&send("sync", { "dma-read" => { addr => $des, len => 8 } });
		$obj = &receive;
		if (!(@a = @{$obj->{'bytes'}})) {
			&dbg("no bytes?\n");
			return;
		}
		$addr = ($a[3] << 24) | ($a[2] << 16) | ($a[1] << 8) | $a[0];
		$len = (($a[5] << 8) | $a[4]);
		$last = $a[6] > 0;
		$trans = ($cur_len, $len)[$len < $cur_len];
		$cur_len -= $trans;
		&dbg("### addr $addr len $len trans $trans remain $cur_len\n");
		$reply = {
			"dma-write" => {
				addr => int($addr),
				len  => int($trans),
				bytes => [map { ($_ % 26) + 65 } $chr..($chr+$trans-1)],
			}
		};
		&send("sync", $reply);
		&receive;
		$reply = {
			"dma-write" => {
				addr => int($des)+4,
				len => 4,
				bytes => [int($trans) & 0xff, int($trans) >> 8, int($a[6]), int($cur_len == 0)],
			}
		};
		&send("sync", $reply);
		&receive;
		$des += 8;
		$chr += $trans;
	} while (!$last);
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
		($old, $CTL) = ($CTL, $v & 1);
		if ($a == 0) {
			$BMPTR = $v;
		} elsif ($a == 4) {
			$CMD = $v;
			if ($CMD == 1) {
				&start_dma;
			}
		}
		&send_ret;
	} elsif ($i = $$obj{'read'}) {
		$a = $i->{'addr'};
		&send_ret if  ($a % 4 || $a >= 16);
		$s = $i->{'size'};
		&send_ret if  ($s != 4);
		&dbg("### $0: reading from $a\n");
		if ($a == 0) {
			&send_ret($BMPTR);
		} elsif ($a == 4) {
			&send_ret($CMD);
		} else {
			&clear_intr;
			&send_ret($STS);
		}
	}			
}
