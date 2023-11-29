import unittest
from chempath.substance import *


class TestDBSubstance(unittest.TestCase):

    def setUp(self):
        self.db = DBSubstance('cas', load_basic_substance=False)

    def test_basic_substance(self):

        self.db = DBSubstance('cas', load_basic_substance=True)

        cas = '100-00-5'
        smiles = 'O=N(=O)C1=CC=C(Cl)C=C1'
        name = '1-Chloro-4-nitrobenzene'
        chinese = '对氯硝基苯'
        formula = 'C<6>H<4>ClNO<2>'

        substance = self.db.get_substance('100-00-5')
        self.assertEqual(substance.cas, cas)
        self.assertEqual(substance.smiles, smiles)
        self.assertEqual(substance.name, name)
        self.assertEqual(substance.chinese, chinese)
        self.assertEqual(substance.formula, formula)
        self.assertEqual(self.db.get_substance(1), None)
        

    def test_basic_id_smiles(self):
        self.db = DBSubstance('smiles', load_basic_substance=True)
        print(self.db.get_substance('[Cl-].ClCC[N+](C)(C)C').to_dict())

    def test_duplicate(self):
        
        cas = '100-00-5'
        smiles = 'test_smiles'
        name = 'test_name'
        chinese = '测试'
        formula = 'test_formula'

        self.db.add_substance(
            cas = cas,
            smiles = smiles,
            name = name,
            chinese = chinese,
            formula = formula,
        )

        size_before = self.db.size
        self.db.add_substance(
            cas = cas,
            smiles = smiles,
            name = name,
            chinese = chinese,
            formula = formula,
        )
        self.assertEqual(self.db.size, size_before)

    def test_empty_identity(self):

        name = 'test_name'
        cas = None
        smiles = 'test_smiles'
        chinese = '测试'
        formula = 'test_formula'

        size_before = self.db.size

        with self.assertRaises(ValueError) as e:
            self.db.add_substance(
                name,
                cas = cas,
                smiles = smiles,
                chinese = chinese,
                formula = formula
            )

        self.assertEqual(self.db.size, size_before)

    def test_cas_max_length(self):

        name = 'test_name'
        cas = 'AAAAAA-AAAAAAAA-AAAA'
        smiles = 'test_smiles'
        chinese = '测试'
        formula = 'test_formula'

        with self.assertRaises(ValueError) as e:
            self.db.add_substance(
                name,
                cas = cas,
                smiles = smiles,
                chinese = chinese,
                formula = formula
            )

        self.assertEqual(str(e.exception), "invalid CAS number: '{}'!".format(cas))

        cas = 'AAAAA'
        with self.assertRaises(ValueError) as e:
            self.db.add_substance(
                name,
                cas = cas,
                smiles = smiles,
                chinese = chinese,
                formula = formula
            )

        self.assertEqual(str(e.exception), "invalid CAS number: '{}'!".format(cas))
