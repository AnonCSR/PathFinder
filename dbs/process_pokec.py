import sys

# Separate data into multiple CSV files
def separate_data(data_path):
    nodes_set = set()
    with open(data_path, 'r') as data_file, open('pokec_nodes.csv', 'w') as nodes_out, open('pokec_edges.csv', 'w') as edges_out:
        for edge in data_file:
            if edge[0] == '#':  # Skip comments
                continue
            info = edge.strip('\n').split()
            if len(info) != 2:  # Error
                print('Error in format!')
                break
            else:  # Valid edge
                if info[0] not in nodes_set:
                    nodes_set.add(info[0])
                    nodes_out.write(f'{info[0]}\n')
                if info[1] not in nodes_set:
                    nodes_set.add(info[1])
                    nodes_out.write(f'{info[1]}\n')
                edges_out.write(f'{info[0]},{info[1]}\n')

# Generate separate CSV files for Pokec dataset
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        separate_data(input_file)
    except IndexError:
        print('Args are missing!')
