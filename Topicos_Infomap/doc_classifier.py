import os

output_folder = '../Classificação dos Documentos/'
doc_folder = '../Documentos Finais/'

for doc in os.listdir():
    if doc.split('.')[1] != 'ftree':
        continue
    print('--> ' + doc)

    #--------- Carregando Topicos/Clusters ----------#
    f = open(doc, 'r')
    lines = f.readlines()
    f.close()

    n_cluster = 0
    cluster = [] # cada elemento i da lista contém o conjunto de pares termo-flow do cluster i
    flow_cluster = [] # cada elemento i da lista contém o flow total do cluster i
    flow_sum = 0

    for l in lines[2:]:
        if l[0] == '*':
            break

        spl = l.split()
        if int(spl[0].split(':')[0]) == n_cluster:
            cluster[n_cluster-1][spl[2][1:-1]] = float(spl[1])
            flow_sum += float(spl[1]) 
        else:
            n_cluster += 1
            cluster.append({spl[2][1:-1]:float(spl[1])})
            if not n_cluster == 1:
                flow_cluster.append(flow_sum)
            flow_sum = float(spl[1])
    flow_cluster.append(flow_sum)
    #--------- Carregando documentos ----------#
    ego = doc.split('.')[0]
    file = doc_folder + ego + '.txt'
    f = open(file, 'r', encoding="utf8")
    documentos = f.readlines()
    f.close()

    #---------- Montando e salvando matriz de classificação ----------#
    file = output_folder + ego + '.txt'
    output = open(file, 'w+')

    alpha = 0.1 #valor mínimo de fluxo que um tópico precisa em relação ao melhor tópco (poda de tópicos)
    min_fluxo = alpha * flow_cluster[0]
    print('Maior fluxo: ' + str(flow_cluster[0]) + '\nFluxo mínimo: ' + str(min_fluxo) + '\n')

    for doc in documentos:
        termos = doc.split()
        for topic in range(0, n_cluster):

            #--- Verificando o valor de poda de tópicos---#
            if flow_cluster[topic] < min_fluxo:
                break

            count = 0
            flow = 0
            for t in termos:
                if t in cluster[topic].keys():
                    count += 1
                    flow += cluster[topic][t]
            if flow_cluster[topic] == 0 and count > 0:
                k = 100
            elif flow_cluster[topic] == 0:
                k = 0.0
            else:
                k = count * flow/flow_cluster[topic]
            output.write(str(k) + ' ')
        output.write('\n')
    
    output.close()