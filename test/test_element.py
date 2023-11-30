import unittest
from chempath.element import *


class TestElement(unittest.TestCase):

    def test_Element(self):
        element = Element(1)
        self.assertEqual(element.symbol, 'H')
        self.assertEqual(element.name, 'Hydrogen')
        self.assertEqual(element.mass, 1.007)
