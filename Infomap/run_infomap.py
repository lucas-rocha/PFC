import os

graphs_folder = '../Matrizes_de_Coocorrencia/'
output_folder = '../Topicos_Infomap/'

for doc in os.listdir(graphs_folder):
    if doc.split('.')[1] != 'net':
        continue
    print('\n\n--> ' + doc)

    comando = './Infomap --ftree ' + graphs_folder + doc + ' ' + output_folder
    print(comando)
    os.system(comando)
