import unittest

def main():
    loader = unittest.TestLoader()
    tests  = loader.discover('../tests/')
    testRunner = unittest.runner.TextTestRunner()
    testRunner.run(tests)

if __name__ == '__main__':
    main()
