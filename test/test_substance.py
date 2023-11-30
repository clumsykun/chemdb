import unittest
from chempath.substance import *


class TestDBSubstance(unittest.TestCase):

    def test_basic_use_cas(self):

        db = DBSubstance('cas', load_basic_substance=True)

        cas = '100-00-5'
        smiles = 'O=N(=O)C1=CC=C(Cl)C=C1'
        name = '1-Chloro-4-nitrobenzene'
        chinese = '对氯硝基苯'
        formula = 'C<6>H<4>ClNO<2>'

        substance = db.get_substance(cas)
        
        self.assertEqual(substance.identity, cas)
        self.assertEqual(substance.cas, cas)
        self.assertEqual(substance.smiles, smiles)
        self.assertEqual(substance.name, name)
        self.assertEqual(substance.chinese, chinese)
        self.assertEqual(substance.formula, formula)
        self.assertEqual(db.get_substance(1), None)

    def test_basic_use_smiles(self):
        db = DBSubstance('smiles', load_basic_substance=True)

        name = 'Benzaldehyde'
        cas = '100-52-7'
        smiles = 'O=CC=1C=CC=CC1'
        formula = 'C<7>H<6>O'
        chinese = '苯甲醛'

        substance = db.get_substance(smiles)
        self.assertEqual(substance.identity, smiles)
        self.assertEqual(substance.name, name)
        self.assertEqual(substance.cas, cas)
        self.assertEqual(substance.smiles, smiles)
        self.assertEqual(substance.formula, formula)
        self.assertEqual(substance.chinese, chinese)

    def test_basic_use_name(self):
        db = DBSubstance('name', load_basic_substance=True)

        name = 'Benzaldehyde'
        cas = '100-52-7'
        smiles = 'O=CC=1C=CC=CC1'
        formula = 'C<7>H<6>O'
        chinese = '苯甲醛'

        substance = db.get_substance(name)
        self.assertEqual(substance.identity, name)
        self.assertEqual(substance.name, name)
        self.assertEqual(substance.cas, cas)
        self.assertEqual(substance.smiles, smiles)
        self.assertEqual(substance.formula, formula)
        self.assertEqual(substance.chinese, chinese)

    def test_duplicate(self):
        db = DBSubstance('name', load_basic_substance=False)
        cas = '100-00-5'
        smiles = 'test_smiles'
        name = 'test_name'
        chinese = '测试'
        formula = 'test_formula'

        db.add_substance(
            cas = cas,
            smiles = smiles,
            name = name,
            chinese = chinese,
            formula = formula,
        )

        size_before = db.size
        db.add_substance(
            cas = cas,
            smiles = smiles,
            name = name,
            chinese = chinese,
            formula = formula,
        )
        self.assertEqual(db.size, size_before)

    def test_substance_property_None(self):
        db = DBSubstance('cas', load_basic_substance=False)
        name = 'Benzaldehyde'
        cas = '100-52-7'
        smiles = 'O=CC=1C=CC=CC1'
        formula = 'C<7>H<6>O'
        chinese = '苯甲醛'

        db.add_substance(
            name,
            cas = cas,
            smiles = None,
            chinese = chinese,
            formula = formula
        )

        substance = db.get_substance(cas)
        self.assertEqual(substance.smiles, None)
        size_before = db.size

        with self.assertRaises(ValueError) as e:
            db.add_substance(
                name,
                cas = None,
                smiles = smiles,
                chinese = chinese,
                formula = formula
            )

        self.assertEqual(db.size, size_before)

        db = DBSubstance('smiles', load_basic_substance=False)
        with self.assertRaises(ValueError) as e:
            db.add_substance(
                name,
                cas = cas,
                smiles = None,
                chinese = chinese,
                formula = formula
            )
        self.assertEqual(str(e.exception), 'except identity not be None!')
    
        with self.assertRaises(TypeError) as e:
            db.add_substance(
                None,
                cas = cas,
                smiles = None,
                chinese = chinese,
                formula = formula
            )
        self.assertEqual(str(e.exception), 'argument 1 must be str, not None')


    def test_cas_max_length(self):
        db = DBSubstance('name', load_basic_substance=False)
        name = 'test_name'
        cas = 'AAAAAA-AAAAAAAA-AAAA'
        smiles = 'test_smiles'
        chinese = '测试'
        formula = 'test_formula'

        with self.assertRaises(ValueError) as e:
            db.add_substance(
                name,
                cas = cas,
                smiles = smiles,
                chinese = chinese,
                formula = formula
            )

        self.assertEqual(str(e.exception), "invalid CAS number: '{}'!".format(cas))

        cas = 'AAAAA'
        with self.assertRaises(ValueError) as e:
            db.add_substance(
                name,
                cas = cas,
                smiles = smiles,
                chinese = chinese,
                formula = formula
            )

        self.assertEqual(str(e.exception), "invalid CAS number: '{}'!".format(cas))
