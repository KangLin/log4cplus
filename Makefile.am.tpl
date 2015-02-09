[= AutoGen5 template -*- Mode: scheme -*-
am
=][=
(use-modules (ice-9 ftw))
=]## Generated by Autogen from [= (tpl-file) =]

AM_CPPFLAGS = -I$(top_srcdir)/include \
	-I$(top_srcdir)/threadpool \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/catch/single_include \
        -DCATCH_CONFIG_PREFIX_ALL=1 \
	@LOG4CPLUS_NDEBUG@
AM_CXXFLAGS=@LOG4CPLUS_PROFILING_CXXFLAGS@
AM_LDFLAGS=@LOG4CPLUS_PROFILING_LDFLAGS@

ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = ChangeLog log4cplus.pc.in
SUBDIRS = include
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = log4cplus.pc

if ENABLE_VERSION_INFO_OPTION
VERSION_INFO_OPTION=-version-info @LT_VERSION@
else
VERSION_INFO_OPTION=-avoid-version
endif

if ENABLE_RELEASE_OPTION
RELEASE_OPTION=-release @LT_RELEASE@
else
RELEASE_OPTION=
endif

LOG4CPLUS_VERSION_LT_FLAGS=$(VERSION_INFO_OPTION) $(RELEASE_OPTION)

INCLUDES_SRC_PATH = $(top_srcdir)/include/log4cplus
liblog4cplus_la_file = $(top_builddir)/liblog4cplus.la
if BUILD_WITH_WCHAR_T_SUPPORT
liblog4cplusU_la_file = $(top_builddir)/liblog4cplusU.la
endif

# The tests add to this using += operator, so it needs to be initialized to
# empty first.
noinst_PROGRAMS=

[=
(let ((dirs (list "src" 
                  "simpleserver" 
                  "qt4debugappender"
                  "qt5debugappender"
                  "swig"
                  "tests")))
  (for-each (lambda (dir) 
              (letrec ((files (list)))
                ;;(define files (list))
                (define (emit-am-file-ftw-cb filename statinfo flag)
                  (begin
                    (if (and (string-suffix-ci? ".am" filename))
                        (set! files (append! files (list filename))))
                    #t))
                (begin
                  (ftw dir emit-am-file-ftw-cb)
                  (sort! files string<?)
                  (for-each 
                   (lambda (x) 
                     (emit "include %D%/" x "\n"))
                   files)
                  (emit "\n"))))
            dirs))
=]
