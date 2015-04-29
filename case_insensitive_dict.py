#!/usr/bin/env python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>
# Case insensitive dictionary implementation

def noop(*args, **kwargs):
    raise ValueError('noop for this method')

def is_callable_attr(obj, attr):
    retr = getattr(obj, attr, None)
    if retr is None:
        return False
    return hasattr(retr, '__call__')

def generate_lowerer(obj, method_name):
    def _f(self, key, *args, **kwargs):
        if is_callable_attr(key, 'lower'):
            key = key.lower()

        super_method = getattr(self.get_super(), method_name, noop)
        return super_method(key, *args, **kwargs)

    return _f

class CaseAgnosticDict(dict):
    keyable_methods = ('__setitem__', '__getitem__', '__contains__', '__delitem__', 'pop',)

    def get_super(self):
        return super(CaseAgnosticDict, self)

ref = CaseAgnosticDict

# Bind the methods
for method_name in ref.keyable_methods:
    generated = generate_lowerer(ref, method_name)
    setattr(ref, method_name, generated)
 
def main():
    ca_dict = CaseAgnosticDict(dock=10)
    ca_dict['Dock'] = 30
    assert ('dock' in ca_dict) == ('DOcK' in ca_dict), 'case insensitivity expected'
    assert ca_dict.pop('DocK', 1000) == 30, 'expecting the set value'
    assert len(ca_dict) == 0, 'all keys should have been popped'
    ca_dict['10'] = 30
    ca_dict[10] = 30
    assert len(ca_dict) == 2, '2 unique insertions expected'
    ca_dict[10] = 10
    assert ca_dict.get(10, 20) == 10, 'expecting 10 as the get result'
    ca_dict[(10, 20,)] = 'ollo'
    assert ca_dict.get((10, 20,), 'xram') == 'ollo', 'expecting "ollo" as the get result'

if __name__ == '__main__':
    main()
