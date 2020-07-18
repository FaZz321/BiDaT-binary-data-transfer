from BiDaT import *



#########
# Utils #
#########

def write_to_file(file_name, binary_data):
    f = open(file_name, "wb")
    f.write(binary_data)
    f.close()



########
# Init #
########

rec = BiRecord(None)
value = None
binary_data = None

print("Generation started!")


#########################
# Single value examples #
#########################

# Integer:

value = 123571113
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("SingleInteger.bdt", binary_data)


# Real:

value = 3.1415
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("SingleReal.bdt", binary_data)

# Bool:

value = False
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("SingleBoolFalse.bdt", binary_data)

value = True
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("SingleBoolTrue.bdt", binary_data)

# String:

value = "Take it easy!"
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("SingleString.bdt", binary_data)

# Binary:

value = b"\x00\x01\x02\x03\x04\x05"
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("SingleBinary.bdt", binary_data)



##############################
# Single type array examples #
##############################

# Integer:

value = [-2, -1, 1, 2, 3, 5, 7, 11, 13]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("StArrayInteger.bdt", binary_data)


# Real:

value = [1.1, 2.3, -0.4, -3.14]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("StArrayReal.bdt", binary_data)

# Bool:

value = [False, True, False, False, True, True]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("StArrayBool.bdt", binary_data)

# String:

value = ["Easy", "take", "it!"]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("StArrayString.bdt", binary_data)

# Binary:

value = [b"\x05", b"\x04", b"\x03", b"\x02", b"\x01", b"\x00"]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("StArrayBinary.bdt", binary_data)



##################################
# Different types array examples #
##################################

# simple types

value = [7, False, 3.14]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("DtArray1.bdt", binary_data)

value = [2.34, True, 21, 1.23]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("DtArray2.bdt", binary_data)

# complex and simple types:

value = [1, "xxx", 2, "yyy", 3, "zzz", 4, b"\x01", b"\x88", b"\xff"]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("DtArray3.bdt", binary_data)

value = ["array", 2, b"\x77"]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("DtArray4.bdt", binary_data)



#################
# Nested arrays #
#################

# two layers:

value = [[1,2], [3,4]]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("NestedArray1.bdt", binary_data)

value = [[1.2, 2.3, True],["some_string", b"\xab\x00\x77"]]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("NestedArray2.bdt", binary_data)

# three layers, with scalar varibles:

value = [
    [3.14, "some_string" ],
    [
        ["some_string_2", False, 1.234567],
        b"\x01\x02\x03"
    ],
    123
]
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("NestedArray3.bdt", binary_data)

# empty list:

value = []
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("EmptyList.bdt", binary_data)

# empty named list:

value = {}
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("EmptyNamedList.bdt", binary_data)

# empty binary data:

value = b""
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("EmptyBinary.bdt", binary_data)

# big one:

value = []
temp = []
for i in range(0, 1000):
    for j in range(0, 1000):
        temp.append(i*j)
    value.append(temp)
    temp = []
rec.changeRootValue(value)
binary_data = rec.encode()
write_to_file("NestedArray4.bdt", binary_data)


print("Generation completed!")
