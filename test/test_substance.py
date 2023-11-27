from pprint import pprint
from chempath.element import PERIODIC_TABLE

for symbol, elem in PERIODIC_TABLE.items():
    print(symbol, elem, elem.chinese, elem.__module__)

    
