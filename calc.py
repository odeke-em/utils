#!/usr/bin/env python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>

# Really naive calculator that assumes no order of precedence and
# sort of like using parentheses to keep order of evaluation
# Just implemented as a warm up for an exam/code kata.

import operator as op

Operand  = 0
Operator = 1
Unary = 1
Binary = 2

operators = {
    '*':    (Binary,            { Binary: op.mul }, float,),
    '/':    (Binary,            { Binary: op.floordiv }, float,),
    '-':    (Binary | Unary,    { Binary: op.sub, Unary: op.neg }, int,),
    '+':    (Binary | Unary,    { Binary: op.add, Unary: op.pos }, int,),
    '^':    (Binary,            { Unary: op.xor, Binary: op.pow }, int,),
    '~':    (Unary,             { Unary: lambda x: ~x }, int,),
    '&':    (Binary,            { Binary: op.iand }, int,),
    '&&':   (Binary,            { Binary: op.and_ }, int,),
    'and':  (Binary,            { Binary: op.and_ }, int,),
}

def classify(v):
    if v in operators:
        return Operator

    return Operand

def parser(seq_str):
    operand_stack  = []
    operator_stack = []

    seq_str = seq_str.split(' ')
    splits = [v for v in seq_str if v]

    for v in splits:
        op_type = classify(v)
        selector = operand_stack

        if op_type == Operator:
            selector = operator_stack
            
        selector.append(v)

    return evaluate(operator_stack, operand_stack)

def popper(operator, operand_stack):
    op_behaviour, op_func_morphism, for_each, *rest = operators.get(operator, (None, None, None,))
    if op_func_morphism is None:
        return Exception('"%s" is an undefined operator! Aborting\n'%(v)), False

    stack_size = len(operand_stack)
    func = None
    op_count = -1

    if op_behaviour & Binary:
        if stack_size < 2 and op_behaviour & Unary:
            op_count = 1
            func = op_func_morphism.get(Unary, None)
        else:
            op_count = 2
            func = op_func_morphism.get(Binary, None)
    else:
        op_count = 1
        func = op_func_morphism.get(Unary, None)

    if stack_size < op_count:
        return Exception('"%s" expects %d operands'%(operator, op_count)), False

    # Popping time
    args = []
    for i in range(op_count):
        args.append(operand_stack.pop(0))

    mapped_args = list(map(lambda k: for_each(k), args))
    result = func(*mapped_args)

    return result, True

def _evaluate(operator_stack, operand_stack):
    for operator in operator_stack:
        result, ok = popper(operator, operand_stack)
        if not ok:
            return result

        # Push the result back to the top of the stack
        operand_stack.insert(0, result)

    assert len(operand_stack) == 1, "Expecting only one value at the end"
    result = operand_stack.pop(0)
    return result, True

def evaluate(*args, **kwargs):
    result, _ = _evaluate(*args, **kwargs)
    return result

def main():
    print(parser('~ 2 - + -2'))
    print(parser('1 && 1'))
    print(parser('~ 271 * 262 * 918 - 10029'))
    print(parser('1 && 0'))
    print(parser('1 and 0'))

if __name__ == '__main__':
    main()
