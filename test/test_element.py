import unittest
from chempath.element import *


class TestElement(unittest.TestCase):
    
    def test_Element(self):
        self.assertEqual(Element(1).symbol, 'H')
        pass
