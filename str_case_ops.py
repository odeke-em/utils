#!/usr/bin/env python

# Copyright 2014 Emmanuel Odeke
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Module to enable string operations eg
#     case modifications ie lower(), upper()
#  on a collection of items.

EXCLUDE_NONE = 0
EXCLUDE_LEFT = 1
EXCLUDE_RIGHT = 2

is_callable = lambda x: x and hasattr(x, '__call__')

default_joiner = lambda seq: '='.join(seq)
default_splitter = lambda splittable: splittable.split('=') \
                        if is_callable_attr(splittable, 'split') else splittable


def is_callable_attr(obj, attr):
    value = getattr(obj, attr, None)
    if not value:
        return False

    return is_callable(value)


def _invoke_case_mod(case_func_name, item, *args, **kwargs):
    if not is_callable_attr(item, case_func_name):
       return item

    return getattr(item, case_func_name)(*args, **kwargs)


def lower(*args, **kwargs):
    return _invoke_case_mod('lower', *args, **kwargs)


def capitalize(*args, **kwargs):
    return _invoke_case_mod('capitalize', *args, **kwargs)


def upper(*args, **kwargs):
    return _invoke_case_mod('upper', *args, **kwargs)


def translate_skip_to_range(seq_len, offset=0, skip_const=EXCLUDE_NONE):
    if seq_len <= 1:
        return range(0, seq_len), 1, range(0), 1
    elif skip_const == EXCLUDE_RIGHT:
        return range(0 + offset, seq_len - 1), 1, range(seq_len - 1, seq_len), 0
    elif skip_const == EXCLUDE_LEFT:
        return range(offset, 1), 0, range(1 + offset, seq_len), 1
    else:
        return range(seq_len), 1, range(0), 1 


def _caseify(
        func, iterator, splitter=default_splitter,
        joiner=default_joiner, skip_const=EXCLUDE_LEFT, offset=0):

    if not is_callable_attr(iterator, '__iter__'):
        return iterator

    tokens = []
    
    for it in iterator:
        sub_items = []
        split_items = splitter(it)
        if not is_callable_attr(it, '__len__'):
            tokens.append(split_items)
            continue

        split_len = len(split_items)

        l_iter, op_on_lhs, r_iter, op_on_rhs = translate_skip_to_range(
                                       split_len, offset, skip_const=skip_const)

        if op_on_lhs:
            for i in l_iter:
                sub_items.append(func(split_items[i]))
        else:
            for i in l_iter:
                sub_items.append(split_items[i])

        if op_on_rhs:
            for j in r_iter:
                sub_items.append(func(split_items[j]))
        else:
            for j in r_iter:
                sub_items.append(split_items[j])

        tokens.append(joiner(sub_items))

    return tokens


def lower_args(*args, **kwargs):
    return _caseify(lower, *args, **kwargs)


def upper_args(*args, **kwargs):
    return _caseify(upper, *args, **kwargs)


def capitalize_args(*args, **kwargs):
    return _caseify(capitalize, *args, **kwargs)


def main():
    init_args = [
        '--local_queue_workers',
        '--phantomjs_binary=$PHANTOMJS_BINARY',
        '--phantomjs_script=$CAPTURE_SCRIPT',
        '--phantomJs_timeout=20',
        '--release_server_prefix=http://localhost:5000/api',
        '--queue_server_prefix=http://localhost:5000/api/work_queue',
        '--queue_idle_poll_seconds=10',
        '--queue_busy_poll_seconds=10',
        '--pdiff_timeout=20',
        '--reload_code',
        '--inspectJS',
        '--port=5000',
        '--verbose',
        '--ignore_auth'
    ]

    args = init_args[:]

    joiner = default_joiner
    splitter = default_splitter

    expected_right_excluded = [
        '--local_queue_workers',
        '--phantomjs_binary=$PHANTOMJS_BINARY',
        '--phantomjs_script=$CAPTURE_SCRIPT',
        '--phantomjs_timeout=20',
        '--release_server_prefix=http://localhost:5000/api',
        '--queue_server_prefix=http://localhost:5000/api/work_queue',
        '--queue_idle_poll_seconds=10',
        '--queue_busy_poll_seconds=10',
        '--pdiff_timeout=20',
        '--reload_code',
        '--inspectjs',
        '--port=5000',
        '--verbose',
        '--ignore_auth'
    ]
    right_excluded = lower_args(args, splitter, joiner, EXCLUDE_RIGHT)

    assert args == init_args, 'No mutations expected'
    assert right_excluded == expected_right_excluded

    expected_left_excluded_lower = [
        '--local_queue_workers',
        '--phantomjs_binary=$phantomjs_binary',
        '--phantomjs_script=$capture_script',
        '--phantomJs_timeout=20',
        '--release_server_prefix=http://localhost:5000/api',
        '--queue_server_prefix=http://localhost:5000/api/work_queue',
        '--queue_idle_poll_seconds=10',
        '--queue_busy_poll_seconds=10',
        '--pdiff_timeout=20',
        '--reload_code',
        '--inspectjs',
        '--port=5000',
        '--verbose',
        '--ignore_auth'
    ]
    left_excluded_lower = lower_args(args, splitter, joiner, EXCLUDE_LEFT)

    assert args == init_args, 'No mutations expected'
    assert left_excluded_lower == expected_left_excluded_lower

    expected_none_excluded_upper = [
        '--LOCAL_QUEUE_WORKERS',
        '--PHANTOMJS_BINARY=$PHANTOMJS_BINARY',
        '--PHANTOMJS_SCRIPT=$CAPTURE_SCRIPT',
        '--PHANTOMJS_TIMEOUT=20',
        '--RELEASE_SERVER_PREFIX=HTTP://LOCALHOST:5000/API',
        '--QUEUE_SERVER_PREFIX=HTTP://LOCALHOST:5000/API/WORK_QUEUE',
        '--QUEUE_IDLE_POLL_SECONDS=10',
        '--QUEUE_BUSY_POLL_SECONDS=10',
        '--PDIFF_TIMEOUT=20',
        '--RELOAD_CODE',
        '--INSPECTJS',
        '--PORT=5000',
        '--VERBOSE',
        '--IGNORE_AUTH'
    ]

    all_upperfied = upper_args(args, splitter, joiner, EXCLUDE_NONE)

    assert args == init_args, 'No mutations expected'
    assert all_upperfied == expected_none_excluded_upper, all_upperfied

    expected_left_excluded_upper = [
        '--LOCAL_QUEUE_WORKERS',
        '--phantomjs_binary=$PHANTOMJS_BINARY',
        '--phantomjs_script=$CAPTURE_SCRIPT',
        '--phantomJs_timeout=20',
        '--release_server_prefix=HTTP://LOCALHOST:5000/API',
        '--queue_server_prefix=HTTP://LOCALHOST:5000/API/WORK_QUEUE',
        '--queue_idle_poll_seconds=10',
        '--queue_busy_poll_seconds=10',
        '--pdiff_timeout=20',
        '--RELOAD_CODE',
        '--INSPECTJS',
        '--port=5000',
        '--VERBOSE',
        '--IGNORE_AUTH'
    ]

    left_excluded_upper = upper_args(args, splitter, joiner, EXCLUDE_LEFT)

    assert args == init_args, 'No mutations expected'
    assert left_excluded_upper == expected_left_excluded_upper

    none_with_none = upper_args(None, splitter, joiner, EXCLUDE_LEFT)
    assert none_with_none == None

    for i in range(10):
        assert upper_args(i, splitter, joiner, EXCLUDE_LEFT) == i

    range_iter = range(305)
    assert upper_args(range_iter, splitter, joiner, EXCLUDE_LEFT) == list(range(305))

    args_in = [12, 'first=emmanuel', None]
    mixed_none_excluded = upper_args(args_in, splitter, joiner, EXCLUDE_NONE)
    assert mixed_none_excluded == [12, 'FIRST=EMMANUEL', None]


if __name__ == '__main__':
    main()
