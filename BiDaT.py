################
# BiDaT parser #
################

# USAGE:
#
#   decoding:
#     1. Create new instance of BiParser, using byte sequence as an argument
#     2. use BiParser.parseRecord() to parse byte sequence to BiRecord
#     3. use BiRecord.getRootValue() to get reference (!) to python-type values contained in BiRecord instance
#
#   encoding:
#     1. Create a new instance of BiRecord using root value (python-type data: int, float, bool, str, list, dict, byte, bytearray) as an argument
#     2. use BiRecord.encodeRecord() to create BiDaT byte sequence
#     Note: you can change root value by using BiRecord.changeRootValue() OR you can change it by using reference to root value.
#
#   Not threadsafe. (good thing to add in the future)

import struct

class BiParser:

    def __init__(self, byte_seq):
        self.byte_seq = byte_seq
        self.cursor = 0

    def changeByteSeq(self, new_byte_seq):
        self.byte_seq = new_byte_seq
        self.cursor = 0

    def parseRecord(self):
        if self.byte_seq[self.cursor] != 0x00:
            raise Exception("[BiParser]: frist bytes not found")
        self.cursor += 1
        
        value = self.parseValue()
        
        if self.byte_seq[self.cursor] != 0xFF:
            raise Exception("[BiParser]: last bytes not found")
        self.cursor += 1
        
        result = BiRecord(value)
        return result

    def parseValue(self):
        value_type = self.byte_seq[self.cursor]
        self.cursor += 1

        value_type

        if value_type == 0x01:
            return self.parseInt()
        elif value_type == 0x02:
            return self.parseReal()
        elif value_type == 0x03:
            return self.parseBool()
        elif value_type == 0x04:
            return self.parseString()
        elif value_type == 0x05:
            return self.parseList(large = False)
        elif value_type == 0x15:
            return self.parseList(large = True)
        elif value_type == 0x06:
            return self.parseNList(large = False)
        elif value_type == 0x16:
            return self.parseNList(large = True)
        elif value_type == 0x07:
            return self.parseBinary(large = False)
        elif value_type == 0x17:
            return self.parseBinary(large = True)
        else:
            raise Exception("[BiParser]: wrong value type")

        return result

    def parseInt(self):
        b = self.byte_seq[self.cursor : self.cursor + 4]
        self.cursor += 4
        return struct.unpack("<i", b)[0]

    def parseReal(self):
        b = self.byte_seq[self.cursor : self.cursor + 8]
        self.cursor += 8
        return struct.unpack("<d", b)[0]

    def parseBool(self):
        b = self.byte_seq[self.cursor]
        self.cursor += 1
        
        if (b == 0x01):
            result = True
        elif (b == 0x00):
            result = False
        else:
            raise Exception("[BiParser]: abnormal bool value")
        
        return result

    def parseString(self):
        counter = 0
        byte = None
        while True:
            byte = self.byte_seq[self.cursor]
            self.cursor += 1
            if byte != 0:
                counter += 1
            else:
                break

        b = self.byte_seq[self.cursor - counter - 1 : self.cursor - 1]
        
        return str(b, 'utf-8')

    def parseList(self, large):
        if large:
            size = self.byte_seq[self.cursor : self.cursor+4]
            size = struct.unpack("<I", size)[0]
            self.cursor += 4
        else:
            size = self.byte_seq[self.cursor : self.cursor+1]
            size = struct.unpack("<B", size)[0]
            self.cursor += 1

        result = []
        for i in range(size):
            result.append(self.parseValue())

        return result

    def parseNList(self, large):
        if large:
            size = self.byte_seq[self.cursor : self.cursor+4]
            size = struct.unpack("<I", size)[0]
            self.cursor += 4
        else:
            size = self.byte_seq[self.cursor : self.cursor+1]
            size = struct.unpack("<B", size)[0]
            self.cursor += 1

        result = {}
        for i in range(size):
            key = self.parseString()
            if key in result.keys():
                raise Exception("[BiParser]: keys in named list are not unique")
            else:
                nlist_value = self.parseValue()
            result[key] = nlist_value

        return result

    def parseBinary(self, large):
        if large:
            size = self.byte_seq[self.cursor : self.cursor+4]
            size = struct.unpack("<I", size)[0]
            self.cursor += 4
        else:
            size = self.byte_seq[self.cursor : self.cursor+1]
            size = struct.unpack("<B", size)[0]
            self.cursor += 1

        result = self.byte_seq[self.cursor : self.cursor + size]
        self.cursor += size
        return result

class BiRecord:
    
    def __init__(self, root_value):
        self.root_value = root_value

    def getRootValue(self):
        return self.root_value

    def changeRootValue(self):
        self.root_value = root_value

    def encodeRecord(self):
        result = bytearray()
        
        result.append(0x00)  # starting bytes
        value = self.encodeValue(self.root_value)
        result += value
        result.append(0xFF)  # final bytes
        
        return result

    def encodeValue(self, value):
        result = bytearray()

        t = type(value)

        if t == int:
            result.append(0x01)
            result += self.encodeInt(value)
        elif t == float:
            result.append(0x02)
            result += self.encodeReal(value)
        elif t == bool:
            result.append(0x03)
            result += self.encodeBool(value)
        elif t == str:
            result.append(0x04)
            result += self.encodeString(value)
        elif t == list:
            # type added inside self.encodeList
            result += self.encodeList(value)
        elif t == dict:
            # type added inside self.encodeNList
            result += self.encodeNList(value)
        elif t == bytearray or t == bytes:
            # type added inside self.encodeBinary
            result += self.encodeBinary(value)
        else:
            raise Exception("Only int, float, bool, str, list, dict, byte and bytearray types are allowed")

        return result

    def encodeInt(self, value):
        return struct.pack("<i", value)

    def encodeReal(self, value):
        return struct.pack("<d", value)

    def encodeBool(self, value):
        if (value == True):
            return bytearray([1])
        else:
            return bytearray([0])

    def encodeString(self, value):
        return bytearray(value, "utf-8") + bytearray([0])

    def encodeList(self, value):
        result = bytearray()
        size = len(value)

        if size > 255:
            result.append(0x15)
            result += struct.pack("<I", size)
        else:
            result.append(0x05)
            result += struct.pack("<B", size)
        for i in range(size):
            result += self.encodeValue( value[i] )

        return result

    def encodeNList(self, value):
        result = bytearray()
        size = len(value)

        if size > 255:
            result.append(0x16)
            result += struct.pack("<I", size)
        else:
            result.append(0x06)
            result += struct.pack("<B", size)

        keys = list(value.keys())
        for i in range(size):
            key = keys[i]
            if (type(key) != str):
                raise Exception("Only str based keys allowed")
            nlist_value = self.encodeValue(value[key])
            key = self.encodeString(key);
            result += key + nlist_value

        return result

    def encodeBinary(self, value):
        result = bytearray()
        size = len(value)
        
        if size > 255:
            result.append(0x17)
            result += struct.pack("<I", size)
        else:
            result.append(0x07)
            result += struct.pack("<B", size)
        
        result += value
        return result
