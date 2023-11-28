import unittest
from chempath.substance import *


class TestDBSubstance(unittest.TestCase):

    def setUp(self):
        self.db = DBSubstance(load_basic_substance=True)

    def test_basic_substance(self):

        cas = '100-00-5'
        smiles = 'O=N(=O)C1=CC=C(Cl)C=C1'
        chem_name = '1-Chloro-4-nitrobenzene'
        chem_chinese = '对氯硝基苯'
        formula = 'C<6>H<4>ClNO<2>'

        substance = self.db.get_substance('100-00-5')
        self.assertEqual(substance.cas, cas)
        self.assertEqual(substance.smiles, smiles)
        self.assertEqual(substance.chem_name, chem_name)
        self.assertEqual(substance.chem_chinese, chem_chinese)
        self.assertEqual(substance.formula, formula)

    def test_duplicate(self):
        
        cas = '100-00-5'
        smiles = 'test_smiles'
        chem_name = 'test_name'
        chem_chinese = '测试'
        formula = 'test_formula'

        size_before = self.db.size
        self.db.add_substance(
            cas = cas,
            smiles = smiles,
            chem_name = chem_name,
            chem_chinese = chem_chinese,
            formula = formula,
        )
        self.assertEqual(self.db.size, size_before)

    def test_empty_identity(self):

        cas = None
        smiles = 'test_smiles'
        chem_name = 'test_name'
        chem_chinese = '测试'
        formula = 'test_formula'

        size_before = self.db.size
        # self.assertRaises(
        #     TypeError,
        #     self.db.add_substance(
        #         cas = cas,
        #         smiles = smiles,
        #         chem_name = chem_name,
        #         chem_chinese = chem_chinese,
        #         formula = formula
        #     )
        # )

        self.assertEqual(self.db.size, size_before)
        print(self.db.data)
