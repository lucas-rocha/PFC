import json, os

timelines = "/home/amaury/coleta_old_01/timeline_collect/10mil_egos/json/"
egos = "/home/amaury/graphs/n2/graphs_with_ego/"
output = "/home/amaury/Lucas/n2/egos/"

ego_list = []

for f in os.listdir(egos):
	ego = f.split(".edge_list")
	ego = ego[0]
	ego_list.append(ego)

for ego in ego_list:
	print ("Ego: " + ego)

	doc = timelines + ego + '.json'
	tweets = []
	
	for line in open(doc, "r"):
		tweets.append(json.loads(line))
	
	new_doc =  output + ego + '.txt'
	with open(new_doc, 'a+') as f:
		rt = 'retweeted_status'
		for i in tweets:
			if rt in i.keys():
				autor = str(i[rt]['user']['id'])
				f.write(autor + ' ' + i[rt]['text'] + '\n')


'''
			split = i['text'].split()
			if split[0] == 'RT' and (split[0] == '@' and split[-1] == ':'):
				#print('------------> ' + str(i))
				flag = False
				for w in split:
					if not flag:
						if w == 'RT':
							flag = True
					else:
						if not w == '\n':
							f.write(w +' ')
				f.write(str(i['retweeted_status']['user']['id'])+'\n')
'''
