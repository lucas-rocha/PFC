import os

coocur_folder = '../Matrizes_de_Coocorrencia/'
index_folder = '../Indices\\ de\\ Termos/'
folder = '../Indices de Termos/'

for doc in os.listdir(folder):
    ego = doc.split('.json')[0]
    print("\n--> " + doc)

    file = folder + doc
    f = open(file, 'r')

    n_termos = int(f.readline().split()[-2]) + 1

    f.close()

    comando = './convertGraph ' + index_folder + ego + '.json ' + coocur_folder + ego + '.txt net ' + str(n_termos)
    print(comando)
    os.system(comando)
