import os, sys

if len(sys.argv) != 2:
	raise Exception("Usage: interpret.py input-file")

file_path = sys.argv[1]
with open(file_path, 'r') as f:
	code = f.read().replace(' ', '').replace('\n', '')

def find_bracket_index(s, n):
	lvl, index = 1, 0
	while index < n and lvl != 0:
		if s[index] == '[':
			lvl += 1
		elif s[index] == ']':
			lvl -= 1
		index += 1
	if lvl != 0:
		raise Exception("Brackets do not match")
	return index

def fill_brackets(s, n):
	d = dict()
	for i in range(n):
		if s[i] == '[':
			d[i] = find_bracket_index(s[i+1:], n - i - 1) + i
	return d | {v: k for k, v in d.items()}

num_instructions = len(code)
brackets = fill_brackets(code, num_instructions)

data_size = 30_000
data = [0] * data_size
code_ptr = data_ptr = 0

while code_ptr != num_instructions:
	if code[code_ptr] == '>':
		data_ptr += 1
		if data_ptr == data_size:
			raise OverflowError(f"Data pointer overflow. Fixed data size: {data_size}")
	elif code[code_ptr] == '<':
		data_ptr -= 1
		if data_ptr == -1:
			raise ValueError(f"Negative data pointer (-1).")
	elif code[code_ptr] == '+':
		data[data_ptr] += 1
	elif code[code_ptr] == '-':
		data[data_ptr] -= 1
	elif code[code_ptr] == '.':
		print(chr(data[data_ptr]), end='')
	elif code[code_ptr] == ',':
		data[data_ptr] = ord(input())
	elif code[code_ptr] == '[':
		if data[data_ptr] == 0:
			code_ptr = brackets[code_ptr]
	elif code[code_ptr] == ']':
		if data[data_ptr] != 0:
			code_ptr = brackets[code_ptr]
	else:
		raise Exception(f"Invalid char: {code[code_ptr]}")
	code_ptr += 1
