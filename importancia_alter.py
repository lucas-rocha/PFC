from operator import itemgetter

file = open('40379006.txt', 'r')
tweets = file.readlines()

alters = {}

for i in tweets:
    k = i.split()
    if not k[0] in alters.keys():
        alters[k[0]] = 1
    else:
        alters[k[0]] += 1

print(sorted(alters.items(), key = itemgetter(1), reverse=True))
print(len(alters))