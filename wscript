#!/usr/bin/env python

def depends(ctx):
    ctx('lib-boost-patches')

def options(opt):
    opt.load('compiler_cxx')
    opt.load('gtest')


def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.load('gtest')

    cfg.check_cfg(package='liblog4cxx', uselib_store='LOG4CXX', args=['--cflags', '--libs'])


def build(bld):

    bld(
            target          = 'euter_inc',
            export_includes = '.',
    )

    bld (
            target          = 'euter',
            features        = 'cxx cxxshlib',
            source          =  bld.path.ant_glob('euter/*.cpp'),
            use             = [
                'euter_inc',
                'boost_serialization',
                'mpiconfig',
                'LOG4CXX',
                ],
            install_path    = '${PREFIX}/lib',
            # gccxml requires non-variadic implementation of boost::variant for python wrappers
            defines='BOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES',
            export_defines='BOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES',
    )

    import os
    for test in bld.path.ant_glob('test/*.cpp', excl='gtest-main.cpp'):
        bld(
            target          = 'euter_test_' + os.path.splitext(test.name)[0],
            features        = 'gtest cxx cxxprogram',
            source          = test,
            use             = 'euter',
            install_path    = '${PREFIX}/bin/tests',
        )
