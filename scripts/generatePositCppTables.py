from hardposit import from_bits, from_double
import argparse
import operator
import math

def demologexp(idxa,idxb,nbits,esbits):
    logtab = generate1DLogTable(6,2)
    exptab = generate1DExpTable(6,2)

    a = from_bits(idxa,nbits,esbits)
    b = from_bits(idxb,nbits,esbits)
    sign = 1 if (a.eval() > 0 and b.eval() > 0) or (a.eval() < 0 and b.eval() < 0) else -1
    print("a*b=",(a*b).eval())

    alog = logtab[a.to_bits()]
    blog = logtab[b.to_bits()]

    print("log(a) = ",alog.eval()," log(b) = ",blog.eval())

    ablog = alog + blog

    print("log(a) + log(b) = ",ablog.eval())

    expablog = exptab[ablog.to_bits()]

    print("exp(log(a)+log(b))=",expablog.eval())

    print("sign * exp(log(a)+log(b))=",sign*expablog.eval())

def generateOpTable(nbits, esbits,op):
    posit_range = range(2**nbits)
    table = [ [0]*(2**nbits) for i in posit_range]
    for i in posit_range:
        posit_i = from_bits(i,nbits,esbits)
        for j in posit_range:
            posit_j = from_bits(j,nbits,esbits)
            table[i][j] = op(posit_i,posit_j)
    
    return table


def generate1DLogTable(nbits, esbits):
    posit_range = range(2**nbits)
    table = [0]*(2**nbits)

    for i in posit_range:
        posit_i = from_bits(i,nbits,esbits)
        if posit_i.eval() == 0:
            table[i] = from_bits(2**(nbits-1),nbits,esbits)
            continue
        sign = -1 if posit_i.eval() < 0 else 1
        val = posit_i.eval() if sign > 0 else -posit_i.eval()
        table[i] = from_double(math.log(val),nbits,esbits)
    return table

def generate1DExpTable(nbits, esbits):
    posit_range = range(2**nbits)
    table = [0]*(2**nbits)
    max_pos = from_bits(2**(nbits-1) - 1,nbits,esbits)
    max_log = from_double(2*math.log(max_pos.eval()),nbits,esbits)
    for i in posit_range:
        posit_i = from_bits(i,nbits,esbits)
        if math.fabs(posit_i.eval()) > max_log.eval():
            table[i] = from_bits(2**(nbits-1),nbits,esbits)
            continue
        val = posit_i.eval()
        table[i] = from_double(math.exp(val),nbits,esbits)
    return table

def format2DToCpp(table_2d, table_name, base_type):
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

def format1DToCpp(table_1d,table_name, base_type):
    linear_size = len(table_1d)
    declaration = f'static constexpr {base_type} {table_name}[{linear_size}] = ';
    content = "{";
    for i in range(linear_size):
        p_res = int(table_1d[i].to_hex(),base=16)
        content = content + str(p_res)
        if i < linear_size - 1:
            content = content+","
    content = content+"}"
    return declaration+content

def getType(nbits):
    if nbits <= 8:
        return "int8_t"
    if nbits <= 16:
        return "int16_t"


def formatStructOps(nbits,esbits,logtab):
    declaration = f'template<> struct PositTable<{getType(nbits)},{nbits},{esbits},{"true" if logtab else "false"}>'
    declaration = declaration+"{"
    declaration = declaration+format2DToCpp(generateOpTable(nbits,esbits,operator.add),"add",getType(nbits))+";\n"
    declaration = declaration+format2DToCpp(generateOpTable(nbits,esbits,operator.sub),"sub",getType(nbits))+";\n"
    if not logtab:
        declaration = declaration+format2DToCpp(generateOpTable(nbits,esbits,operator.truediv),"div",getType(nbits))+";\n"
        declaration = declaration+format2DToCpp(generateOpTable(nbits,esbits,operator.mul),"mul",getType(nbits))+";\n"
    else:
        declaration = declaration+format1DToCpp(generate1DLogTable(nbits,esbits),"log",getType(nbits))+";\n"
        declaration = declaration+format1DToCpp(generate1DExpTable(nbits,esbits),"exp",getType(nbits))+";\n"

    declaration = declaration+"};"
    return declaration

parser = argparse.ArgumentParser(prog='generatePositCppTables.py',
                                 description='Generate Cpp operation tables for tabulated backend',
                                 epilog='Text at the bottom of help')

parser.add_argument('-n',help='Number of Posit bits',metavar='nbits',type=int,required=True)
parser.add_argument('-e',help="Number of exponent bits",metavar='esbits',type=int,required=True)
parser.add_argument('--log',help="Number of exponent bits",action='store_true',required=False,default=False)
parser.add_argument('-o',help="Output file",metavar="out.cpp",required=True)

args = parser.parse_args()
nbits = args.n
esbits = args.e
outfile = args.o
logtab = args.log

print(f'Generating source header for table: n={nbits}, esbits={esbits}')
out=formatStructOps(nbits,esbits,logtab)

with open(outfile,"w") as f:
    f.write(out)

