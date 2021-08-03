#!/usr/bin/env python
import sys,os, re, logging, argparse

from pywrap.wrapper import Wrapper
from pywrap import containers, namespaces, matchers

wrap = Wrapper()
mb = wrap.mb

print(dir(mb))

# Special fix up
containers.extend_std_containers(mb)

for ns in ['::boost::serialization', '::boost::archive']:
    try:
        mb.namespace(ns).exclude()
    except matchers.declaration_not_found_t: pass

ns = mb.namespace("::PyNNParameters")
ns.include()

dict_constructor = 'def( "__init__", boost::python::make_constructor(&euter::CellParametersConstructors< {} >::from_dict))'
for c in ns.classes(allow_empty=True):
    c.add_registration_code(dict_constructor.format(c.decl_string))



# expose only public interfaces
namespaces.exclude_by_access_type(mb, ['variables', 'calldefs', 'classes'], 'private')
namespaces.exclude_by_access_type(mb, ['variables', 'calldefs', 'classes'], 'protected')

wrap.set_number_of_files(0)
wrap.finish()
