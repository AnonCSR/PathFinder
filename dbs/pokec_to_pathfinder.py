import sys

# Convert graph edges into the PathFinder format
def convert_db(db_path, edge_label):
    with open(db_path, 'r') as graph, open('pokec_pathfinder.txt', 'w') as pf:
        for edge in graph:
            src, dest = edge.strip('\n').split(',')
            if not src[0].isalpha():
                src = 'P' + src
            if not dest[0].isalpha():
                dest = 'P' + dest
            pf.write(f'{src}->{dest} :{edge_label}\n')

# Convert from Property graph to PathFinder graph format
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        edge_label = sys.argv[2]
        convert_db(input_file, edge_label)
    except IndexError:
        print('Args are missing!')
