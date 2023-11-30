import unittest
from chempath.step import DBStep, DBSubstance


class TestDBSubstance(unittest.TestCase):

    def test_basic(self):
        db = DBStep(DBSubstance('cas'))


        inputs = [
            db.db_substance.get_substance('541-41-3'),
            db.db_substance.get_substance('57946-56-2')
        ]
        target = db.db_substance.get_substance('114108-90-6')
        
        print(inputs)
        print(target)


        # db.add_step(Benzaldehyde, [Benzaldehyde, Benzaldehyde, Benzaldehyde])
