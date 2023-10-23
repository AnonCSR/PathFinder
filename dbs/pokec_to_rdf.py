import sys

# Convert graph edges into RDF triples
def convert_db(db_path, edge_label):
    with open(db_path, 'r') as graph, open('pokec_rdf.nt', 'w') as rdf:
        for edge in graph:
            src, dest = edge.strip('\n').split(',')
            rdf.write(f'<http://p.db/{src}> <http://p.db/{edge_label}> <http://p.db/{dest}> .\n')

# Convert from Property graph to RDF graph format
if __name__ == '__main__':
    try:
        input_file = sys.argv[1]
        edge_label = sys.argv[2]
        convert_db(input_file, edge_label)
    except IndexError:
        print('Args are missing!')
