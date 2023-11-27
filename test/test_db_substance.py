import unittest
from chempath.db_substance import *


class TestDBSubstance(unittest.TestCase):
    
    db = DBSubstance()
    
    def test_Element(self):
        self.assertEqual(self.db.size, 1024)
