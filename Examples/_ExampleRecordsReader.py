from BiDaT import *



#########
# Utils #
#########

def read_from_file(file_name):
    f = open(file_name, "rb")
    binary_data = f.read()
    f.close()
    return binary_data


########
# INIT #
########

file_to_read = "NestedArray4.bdt"


###########
# READING #
###########

print("Reading started!")

binary_data = read_from_file(file_to_read)
parser = BiParser(binary_data)

rec = parser.parseRecord()
value = rec.getRootValue()

print("Reading completed!")
print("Printing...")
print(value)
print("Printing completed!")
