#
# Example script to debug perft results
# You need to paste perft results from
# your engine and Stockfish to the
# variables below to make it work 
# for your own perft results
#

# import json module
import json

# your engine results at given depth
bbc_raw = '''
move: d7d6   nodes: 1
move: d7d5   nodes: 1
move: d7c6   nodes: 1
move: g6g5   nodes: 1
move: b4b3   nodes: 1
move: b4c3   nodes: 1
move: h3g2   nodes: 1
move: b6c8   nodes: 1
move: b6d5   nodes: 1
move: b6a4   nodes: 1
move: b6c4   nodes: 1
move: f6g8   nodes: 1
move: f6h7   nodes: 1
move: f6d5   nodes: 1
move: f6h5   nodes: 1
move: f6e4   nodes: 1
move: f6g4   nodes: 1
move: g7f8   nodes: 1
move: g7h6   nodes: 1
move: a6c8   nodes: 1
move: a6b7   nodes: 1
move: a6b5   nodes: 1
move: a6c4   nodes: 1
move: a8b8   nodes: 1
move: a8c8   nodes: 1
move: a8d8   nodes: 1
move: h8f8   nodes: 1
move: h8g8   nodes: 1
move: h8h7   nodes: 1
move: h8h6   nodes: 1
move: h8h5   nodes: 1
move: h8h4   nodes: 1
move: e7d8   nodes: 1
move: e7f8   nodes: 1
move: e7d6   nodes: 1
move: e8g8   nodes: 1
move: e8c8   nodes: 1
move: e8d8   nodes: 1
move: e8f8   nodes: 1
'''

# Stockfish perft results at the same depth as your engine
sf_raw = '''
b4b3: 1
g6g5: 1
d7d6: 1
d7d5: 1
h3g2: 1
d7c6: 1
b4c3: 1
b6a4: 1
b6c4: 1
b6d5: 1
b6c8: 1
f6e4: 1
f6g4: 1
f6d5: 1
f6h5: 1
f6h7: 1
f6g8: 1
a6c4: 1
a6b5: 1
a6b7: 1
a6c8: 1
g7h6: 1
g7f8: 1
a8b8: 1
a8c8: 1
a8d8: 1
h8h4: 1
h8h5: 1
h8h6: 1
h8h7: 1
h8f8: 1
h8g8: 1
e7c5: 1
e7d6: 1
e7d8: 1
e7f8: 1
e8d8: 1
e8f8: 1
e8g8: 1
e8c8: 1
'''

# parsed results datasets
bbc = {}
sf = {}

# parse results
[bbc.update({move.split()[1]: move.split()[3]}) for move in bbc_raw.split('\n')[1: -1]]
[sf.update({move.split(':')[0]: move.split(': ')[-1]}) for move in sf_raw.split('\n')[1: -1]]

# order moves
bbc = json.loads(json.dumps(bbc, sort_keys=True))
sf = json.loads(json.dumps(sf, sort_keys=True))

# the number of root moves mathes
if (len(bbc.keys()) == len(sf.keys())):
    print('root moves number matches, writing results to "perft_comparison.txt"')
        
    # write comparison to file
    with open('perft_comparison.txt', 'w') as f:
        # write headers
        f.write('Move:   SF   BBC\n')
        f.write('-----------------\n')
     
        # compare moves
        moves = []

        # loop over the range of indexes
        for index in range(0, len(sf.keys())):
            f.write(list(sf.keys())[index] + ': ' + list(sf.values())[index] + ' ' + list(bbc.values())[index] + '\n')

# otherwise
else:
    print('root moves number does not match, please check "bbc.txt" and "sf.txt" manually')
    
    # write 'bbc.txt'
    with open('bbc.txt', 'w') as f1:
        for key, value in bbc.items():
            f1.write(key + ': ' + value + '\n')

    # write 'sf.txt'
    with open('sf.txt', 'w') as f2:
        for key, value in sf.items():
            f2.write(key + ': ' + value + '\n')



