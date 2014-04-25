#!/usr/bin/python
import sys

def get_batch_file(batch_id):
    file = str(batch_id) + '.g'
    print('Writing batch: ' + file)
    return file

if len(sys.argv) < 2:
    print('USAGE: ' + sys.argv[0] + ' <big_graph_file.g> <batch_size>(default=1000')
    sys.exit(1)

f = open(sys.argv[1], 'r')
batch_id = 1
f_out = open(get_batch_file(batch_id), 'w')

if len(sys.argv) > 2:
    batch_size = int(sys.argv[2])
else:
    batch_size = 1000

num_edges_in_batch = 0
num_vertices = 0
num_edges = 0

for line in f:
    f_out.write(line)
    if line[0] == 'e':
        num_edges_in_batch += 1
        num_edges += 1
    else:
        num_vertices += 1
    if num_edges_in_batch == batch_size:
        f_out.close()
        batch_id += 1
        num_edges_in_batch = 0
        f_out = open(get_batch_file(batch_id), 'w')
    
f_out.close()
print('----------------------------------------------')
print('Created ' + str(batch_id-1) + ' batches')
print('Batch size: ' + str(batch_size) + ' edges')
print('Total #vertices: ' + str(num_vertices))
print('Total #edges: ' + str(num_edges))
print('----------------------------------------------')
