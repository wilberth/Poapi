#!/usr/bin/perl
my ($range) = `ip addr show eth0` =~ /inet\s(\S+)/; # my ip including subnet (/24) 
print "scan range: $range for Northern Digital\n";
my $p = '([\.0-9]{7,})\s*\nHost is up \(.+\)\.\nMAC Address: ([\:0-9A-F]+) \(Northern Digital\)';
my $r =`sudo nmap -sn $range`;
my ($ip, $mac) = $r =~ /$p/;

print "Northern Digital SCU, IP: $ip, MAC: $mac\n";
