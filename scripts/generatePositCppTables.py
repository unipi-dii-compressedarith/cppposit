from hardposit import from_bits, from_double
import argparse
import operator


def generateOpTable(nbits, esbits,op):
    posit_range = range(2**nbits)
    table = [ [0]*(2**nbits) for i in posit_range]
    for i in posit_range:
        posit_i = from_bits(i,nbits,esbits)
        for j in posit_range:
            posit_j = from_bits(j,nbits,esbits)
            table[i][j] = op(posit_i,posit_j)
    
    return table

    
def formatToCpp(table_2d, table_name, base_type):
    linear_size = len(table_2d)
    declaration = f'static constexpr {base_type} {table_name}[{linear_size}][{linear_size}] = ';
    content = "{";
    for i in range(linear_size):
        content = content+"{"
        for j in range(linear_size):
            p_res = int(table_2d[i][j].to_hex(),base=16)
            content = content + str(p_res)
            if j < linear_size-1:
                content = content+","
        content = content+"}"
        if i < linear_size - 1:
            content = content+","

    content = content + "}"
    return declaration+content


def getType(nbits):
    if nbits <= 8:
        return "int8_t"
    if nbits <= 16:
        return "int16_t"


def formatStructOps(nbits,esbits):
    declaration = f'template<> struct PositTable<{getType(nbits)},{nbits},{esbits}>'
    declaration = declaration+"{"
    declaration = declaration+formatToCpp(generateOpTable(nbits,esbits,operator.mul),"mul",getType(nbits))+";\n"
    declaration = declaration+formatToCpp(generateOpTable(nbits,esbits,operator.add),"add",getType(nbits))+";\n"
    declaration = declaration+formatToCpp(generateOpTable(nbits,esbits,operator.sub),"sub",getType(nbits))+";\n"
    declaration = declaration+formatToCpp(generateOpTable(nbits,esbits,operator.truediv),"div",getType(nbits))+";\n"

    declaration = declaration+"};"
    return declaration

parser = argparse.ArgumentParser(prog='generatePositCppTables.py',
                                 description='Generate Cpp operation tables for tabulated backend',
                                 epilog='Text at the bottom of help')

parser.add_argument('-n',help='Number of Posit bits',metavar='nbits',type=int,required=True)
parser.add_argument('-e',help="Number of exponent bits",metavar='esbits',type=int,required=True)
parser.add_argument('-o',help="Output file",metavar="out.cpp",required=True)

args = parser.parse_args()

nbits = args.n
esbits = args.e
outfile = args.o
print(f'Generating source header for table: n={nbits}, esbits={esbits}')
out=formatStructOps(nbits,esbits)

with open(outfile,"w") as f:
    f.write(out)

