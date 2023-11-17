#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "dtypes.h"


static element PERIODIC_TABLE[NUM_ELEMENT] = {
    {1, "Hydrogen", "氢", "H", 1.007},
    {2, "Helium", "氦", "He", 4.002},
    {3, "Lithium", "锂", "Li", 6.941},
    {4, "Beryllium", "铍", "Be", 9.012},
    {5, "Boron", "硼", "B", 10.811},
    {6, "Carbon", "碳", "C", 12.011},
    {7, "Nitrogen", "氮", "N", 14.007},
    {8, "Oxygen", "氧", "O", 15.999},
    {9, "Fluorine", "氟", "F", 18.998},
    {10, "Neon", "氖", "Ne", 20.18},
    {11, "Sodium", "钠", "Na", 22.99},
    {12, "Magnesium", "镁", "Mg", 24.305},
    {13, "Aluminum", "铝", "Al", 26.982},
    {14, "Silicon", "硅", "Si", 28.086},
    {15, "Phosphorus", "磷", "P", 30.974},
    {16, "Sulfur", "硫", "S", 32.065},
    {17, "Chlorine", "氯", "Cl", 35.453},
    {18, "Argon", "氩", "Ar", 39.948},
    {19, "Potassium", "钾", "K", 39.098},
    {20, "Calcium", "钙", "Ca", 40.078},
    {21, "Scandium", "钪", "Sc", 44.956},
    {22, "Titanium", "钛", "Ti", 47.867},
    {23, "Vanadium", "钒", "V", 50.942},
    {24, "Chromium", "铬", "Cr", 51.996},
    {25, "Manganese", "锰", "Mn", 54.938},
    {26, "Iron", "铁", "Fe", 55.845},
    {27, "Cobalt", "钴", "Co", 58.933},
    {28, "Nickel", "镍", "Ni", 58.693},
    {29, "Copper", "铜", "Cu", 63.546},
    {30, "Zinc", "锌", "Zn", 65.38},
    {31, "Gallium", "镓", "Ga", 69.723},
    {32, "Germanium", "锗", "Ge", 72.64},
    {33, "Arsenic", "砷", "As", 74.922},
    {34, "Selenium", "硒", "Se", 78.96},
    {35, "Bromine", "溴", "Br", 79.904},
    {36, "Krypton", "氪", "Kr", 83.798},
    {37, "Rubidium", "铷", "Rb", 85.468},
    {38, "Strontium", "锶", "Sr", 87.62},
    {39, "Yttrium", "钇", "Y", 88.906},
    {40, "Zirconium", "锆", "Zr", 91.224},
    {41, "Niobium", "铌", "Nb", 92.906},
    {42, "Molybdenum", "钼", "Mo", 95.96},
    {43, "Technetium", "锝", "Tc", 98.0},
    {44, "Ruthenium", "钌", "Ru", 101.07},
    {45, "Rhodium", "铑", "Rh", 102.906},
    {46, "Palladium", "钯", "Pd", 106.42},
    {47, "Silver", "银", "Ag", 107.868},
    {48, "Cadmium", "镉", "Cd", 112.411},
    {49, "Indium", "铟", "In", 114.818},
    {50, "Tin", "锡", "Sn", 118.71},
    {51, "Antimony", "锑", "Sb", 121.76},
    {52, "Tellurium", "碲", "Te", 127.6},
    {53, "Iodine", "碘", "I", 126.904},
    {54, "Xenon", "氙", "Xe", 131.293},
    {55, "Cesium", "铯", "Cs", 132.905},
    {56, "Barium", "钡", "Ba", 137.327},
    {57, "Lanthanum", "镧", "La", 138.905},
    {58, "Cerium", "铈", "Ce", 140.116},
    {59, "Praseodymium", "镨", "Pr", 140.908},
    {60, "Neodymium", "钕", "Nd", 144.242},
    {61, "Promethium", "钷", "Pm", 145.0},
    {62, "Samarium", "钐", "Sm", 150.36},
    {63, "Europium", "铕", "Eu", 151.964},
    {64, "Gadolinium", "钆", "Gd", 157.25},
    {65, "Terbium", "铽", "Tb", 158.925},
    {66, "Dysprosium", "镝", "Dy", 162.5},
    {67, "Holmium", "钬", "Ho", 164.93},
    {68, "Erbium", "铒", "Er", 167.259},
    {69, "Thulium", "铥", "Tm", 168.934},
    {70, "Ytterbium", "镱", "Yb", 173.054},
    {71, "Lutetium", "镥", "Lu", 174.967},
    {72, "Hafnium", "铪", "Hf", 178.49},
    {73, "Tantalum", "钽", "Ta", 180.948},
    {74, "Wolfram", "钨", "W", 183.84},
    {75, "Rhenium", "铼", "Re", 186.207},
    {76, "Osmium", "锇", "Os", 190.23},
    {77, "Iridium", "铱", "Ir", 192.217},
    {78, "Platinum", "铂", "Pt", 195.084},
    {79, "Gold", "金", "Au", 196.967},
    {80, "Mercury", "汞", "Hg", 200.59},
    {81, "Thallium", "铊", "Tl", 204.383},
    {82, "Lead", "铅", "Pb", 207.2},
    {83, "Bismuth", "铋", "Bi", 208.98},
    {84, "Polonium", "钋", "Po", 210.0},
    {85, "Astatine", "砹", "At", 210.0},
    {86, "Radon", "氡", "Rn", 222.0},
    {87, "Francium", "钫", "Fr", 223.0},
    {88, "Radium", "镭", "Ra", 226.0},
    {89, "Actinium", "锕", "Ac", 227.0},
    {90, "Thorium", "钍", "Th", 232.038},
    {91, "Protactinium", "镤", "Pa", 231.036},
    {92, "Uranium", "铀", "U", 238.029},
    {93, "Neptunium", "镎", "Np", 237.0},
    {94, "Plutonium", "钚", "Pu", 244.0},
    {95, "Americium", "镅", "Am", 243.0},
    {96, "Curium", "锔", "Cm", 247.0},
    {97, "Berkelium", "锫", "Bk", 247.0},
    {98, "Californium", "锎", "Cf", 251.0},
    {99, "Einsteinium", "锿", "Es", 252.0},
    {100, "Fermium", "镄", "Fm", 257.0},
    {101, "Mendelevium", "钔", "Md", 258.0},
    {102, "Nobelium", "锘", "No", 259.0},
    {103, "Lawrencium", "铹", "Lr", 262.0},
    {104, "Rutherfordium", "暂无", "Rf", 261.0},
    {105, "Dubnium", "暂无", "Db", 262.0},
    {106, "Seaborgium", "暂无", "Sg", 266.0},
    {107, "Bohrium", "暂无", "Bh", 264.0},
    {108, "Hassium", "暂无", "Hs", 267.0},
    {109, "Meitnerium", "暂无", "Mt", 268.0},
    {110, "Darmstadtium", "暂无", "Ds", 271.0},
    {111, "Roentgenium", "暂无", "Rg", 272.0},
    {112, "Copernicium", "暂无", "Cn", 285.0},
    {113, "Nihonium", "暂无", "Nh", 284.0},
    {114, "Flerovium", "暂无", "Fl", 289.0},
    {115, "Moscovium", "暂无", "Mc", 288.0},
    {116, "Livermorium", "暂无", "Lv", 292.0},
    {117, "Tennessine", "暂无", "Ts", 295.0},
    {118, "Oganesson", "暂无", "Og", 294.0},
};


static element *
element_locate_by_id(size_t id)
{
    if (0 < id && id <= NUM_ELEMENT)
        return PERIODIC_TABLE + id - 1;
    
    else
        return NULL;
}

PyObject *
PyLong_num_element()
{
    return (PyObject *) PyLong_FromLong(NUM_ELEMENT);
}

PyObject *
Element_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Element *self;
    self = (Element *) type->tp_alloc(type, 0);

    if (self)
        return (PyObject *) self;
    else
        return NULL;
}

int
Element_init(Element *self, PyObject *args)
{
    int id;

    if (!PyArg_ParseTuple(args, "i", &id))
        return -1;

    self->elem = element_locate_by_id((size_t)id);

    if (!self->elem) {
        PyErr_Format(PyExc_ValueError, "Illegal atomic number: %d!", id);
        return -1;
    }

    return 0;
}

void
Element_dealloc(Element *self)
{
    self->elem = NULL;
    Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject *
Element_id(Element *self, void *closure)
{
    PyObject *o = PyLong_FromSize_t(self->elem->id);
    return (PyObject *) o;
}

PyObject *
Element_name(Element *self, void *closure)
{
    PyObject *o = PyUnicode_FromString(self->elem->name);
    return (PyObject *) o;
}

PyObject *
Element_chinese(Element *self, void *closure)
{
    PyObject *o = PyUnicode_FromString(self->elem->chinese);
    return (PyObject *) o;
}

PyObject *
Element_mass(Element *self, void *closure)
{
    PyObject *o = PyFloat_FromDouble(self->elem->mass);
    return (PyObject *) o;
}

PyObject *
Element_symbol(Element *self, void *closure)
{
    PyObject *o = PyUnicode_FromString(self->elem->symbol);
    return (PyObject *) o;
}

PyObject *
Element_str(Element *self)
{
    return PyUnicode_FromFormat("<Element: (%d) %s>", self->elem->id, self->elem->name);
}
