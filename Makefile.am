## Process this file with automake to produce Makefile.in.

SUBDIRS = lib qpb fpm @ZEBRA@ @BGPD@ @RIPD@ @RIPNGD@ @OSPFD@ @OSPF6D@ @NHRPD@ \
         @ISISD@ @PIMD@ @WATCHQUAGGA@ @VTYSH@ @OSPFCLIENT@ m4 @pkgsrcdir@ \
         tests

DIST_SUBDIRS = lib qpb fpm zebra bgpd ripd ripngd ospfd ospf6d nhrpd \
	  isisd watchquagga vtysh ospfclient m4 pkgsrc tests
	  pimd

EXTRA_DIST = aclocal.m4 SERVICES TODO REPORTING-BUGS INSTALL.quagga.txt \
	update-autotools \
	vtysh/Makefile.in vtysh/Makefile.am \
	tools/zebra.el tools/multiple-bgpd.sh

ACLOCAL_AMFLAGS = -I m4
