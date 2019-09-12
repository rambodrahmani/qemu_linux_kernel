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


my $CTL = 0;
my $STS = 1;
my $RBR = 65;
my $pend_intr;

sub dbg {
	my $msg = shift;
	print "$0: $msg" if $debug;
}

sub send {
	my $msg = shift;
	my $r = $json->encode($msg);
	&dbg("### $0: sending '$r'\n");
	print $out "$r\n";
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
	&send({ return => int($arg) });
}

sub send_intr {
	if (!$pend_intr) {
		&send({ "raise-irq" => 1 });
		&receive;
		&send({ "lower-irq" => 1 });
		&receive;
		$pend_intr = 1;
	}
}

sub clear_intr {
	if ($pend_intr) {
		$pend_intr = 0;
	}
}

sub new_value {
	$RBR++;
	if ($RBR > 90) {
		$RBR = 65;
	}
}

while (1) {
	$obj = &receive;
	if ($$obj{'get-config'}) {
		&send($config);
		&send_ret;
	} elsif ($i = $$obj{'write'}) {
		$a = $i->{'addr'};
		$s = $i->{'size'};
		$v = $i->{'val'};
		if  ($a != 0 || $s != 1) {
			&dbg("scrittura di $v ($s byte) su $a ignorata\n");
			&send_ret;
		} else {
			&dbg("### $0: writing $v into CTL\n");
			($old, $CTL) = ($CTL, $v & 1);
			&dbg("### CTL $old -> $CTL\n");
			if ($old ^ $v) {
				# we are changing the interrupt enabled bit
				if ($old) {
					# was enabled, clear any pending intr
					&clear_intr
				} else {
					# was disabled, send intr if necessary
					&send_intr
				}
			}
			&send_ret;
		}
	} elsif ($i = $$obj{'read'}) {
		$a = $i->{'addr'};
		&send_ret if  ($a % 4 || $a >= 12);
		$s = $i->{'size'};
		&send_ret if  ($s != 1);
		&dbg("### $0: reading from $a\n");
		if ($a == 0) {
			&send_ret($CTL);
		} elsif ($a == 4) {
			&send_ret($STS);
		} elsif ($a == 8) {
			&clear_intr;
			$cur = $RBR;
			&new_value;
			if ($CTL & 1) {
				&send_intr;
			}
			&send_ret($cur);
		} else {
			&send_ret;
		}
	}
}
