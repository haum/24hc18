#!/usr/bin/env perl

my $isAnt = 0;
while ($var = <STDIN>) {
	if ($var eq "END\n") {
		if ($isAnt) {
			print "SUICIDE\n";
		}
		print "END\n";
		$| = 1;
		$isAnt = 0;
	} elsif ($var eq "BEGIN ANT\n") {
		$isAnt = 1;
	}
}
