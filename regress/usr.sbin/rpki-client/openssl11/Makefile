# $OpenBSD: Makefile,v 1.12 2022/10/14 17:17:13 tb Exp $

LDADD +=	-Wl,-rpath,/usr/local/lib/eopenssl11 -L/usr/local/lib/eopenssl11
CFLAGS +=	-I${.CURDIR}/ -I/usr/local/include/eopenssl11/
CFLAGS +=	-DLIBRESSL_INTERNAL

# For mft.c we need ASN1_time_parse() and ASN1_time_tm_cmp() from LibreSSL

# Provide a missing prototype
a_time_tm_gen.c: a_time_tm.c
	echo '#include <openssl/asn1.h>\n' > $@.tmp
	echo '#include "bytestring.h"\n' >> $@.tmp
	echo '#define ASN1error(err) ASN1err(0, (err));' >> $@.tmp
	cat $> >> $@.tmp
	mv -f $@.tmp $@

CLEANFILES += a_time_tm_gen.c a_time_tm_gen.c.tmp

SRCS_test-ip =		a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-mft =		a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-roa =		a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-cert =	a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-gbr =		a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-tal =		a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-bgpsec =	a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-rrdp =	a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-rsc =		a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
SRCS_test-aspa =	a_time_tm_gen.c o_time.c bs_ber.c bs_cbb.c bs_cbs.c
CFLAGS +=	-I${.CURDIR}/../../../../lib/libcrypto/
CFLAGS +=	-I${.CURDIR}/../../../../lib/libcrypto/bytestring

.PATH:		${.CURDIR}/..
.PATH:		${.CURDIR}/../../../../lib/libcrypto
.PATH:		${.CURDIR}/../../../../lib/libcrypto/asn1
.PATH:		${.CURDIR}/../../../../lib/libcrypto/bytestring

.include <bsd.regress.mk>
