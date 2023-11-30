import unittest
from chempath.step import DBStep, DBSubstance


class TestDBSubstance(unittest.TestCase):

    def setUp(self):
        self.db_step = DBStep(DBSubstance('cas'))

    def test_basic(self):
        # print(self.db_step)
        pass
