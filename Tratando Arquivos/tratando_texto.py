#Tokenize sentence with NLTK; Removing special characters and stop-words.
def tokenize(sentence):
	from nltk.corpus import stopwords
	from nltk.tokenize import RegexpTokenizer
	#import nltk	
	#nltk.download('punkt')
	#nltk.download('stopwords')

	stop_words = set(stopwords.words('english'))
	#stop_words = set(stopwords.words('portuguese'))

	'''
	if len(sentence) > 0:
		autor = (sentence.split())[0] #mantendo o @autor
		sentence = sentence.replace(autor,'@')
		autor = autor[:-1] #mantendo o @autor
	'''

	tokenizer = RegexpTokenizer(r'\w+')
	sentence = tokenizer.tokenize(sentence)

	'''
	if len(sentence) > 0:
		sentence.append(autor) #mantendo o @autor
	'''

	tokens = [w for w in sentence if not w in stop_words]

	return tokens

#-------------------------------------------------------------------------#

def remove_links(sentence):

	no_links = ""
	for w in sentence.split():
		if not "http" in w:
			no_links += str(w + " ")

	return no_links

#-------------------------------------------------------------------------#

def remove_mentions(sentence):
	no_mentions = ""

	'''
	if len(sentence.split()) > 0:  
		no_mentions = sentence.split()[0] + " "
	'''
	
	for w in sentence.split():
		if not w[0] == '@':
			no_mentions += str(w + " ")

	return no_mentions

#-------------------------------------------------------------------------#

def remove_hashtags(sentence):
	no_hashtags = ""
	for w in sentence.split():
		if not w[0] == '#':
			no_hashtags += str(w + " ")

	return no_hashtags

#-------------------------------------------------------------------------#

def issues(sentence):
	no_issues = ""
	issues = ['-&gt;','&amp;']
	for w in sentence.split():
		flag = False
		for i in issues:
			if i in w:
				flag = True
				break
		if not flag:
			no_issues += str(w + " ")

	return no_issues

#-------------------------------------------------------------------------#

def lemmatizer(tokens):
	from nltk.stem.wordnet import WordNetLemmatizer
	#import nltk
	#nltk.download('wordnet')
	lmtzr = WordNetLemmatizer()
	lemma = []
	for w in tokens:
		lemma.append(lmtzr.lemmatize(w,'v'))
		
	return lemma

#-------------------------------------------------------------------------#

def save_tokens(id, sentence, rem_links = False, rem_mentions = False, rem_hashtags = False):
	output = "/home/amaury/Lucas/n2/textos_tradados/egos/" # Altere para a pasta de output
	if rem_mentions:
		sentence = remove_mentions(sentence)
	if rem_hashtags:
		sentence = remove_hashtags(sentence)
	if rem_links:
		sentence = remove_links(sentence)

	sentence = issues(sentence)

	tokens = tokenize(sentence)
	if len(tokens) == 0:
		return
	tokens = lemmatizer(tokens)

	doc_save = output + id + '.txt'
	with open(doc_save, 'a+') as f:
		for w in tokens:
			f.write(str(w.upper() + " "))
		f.write(str("\n"))

#-------------------------------------------------------------------------#

def main():
	import os
	folder = "/home/amaury/Lucas/n2/egos/" # Altere para a pasta contendo os arquivos com os tweets de cada rede ego.
	for doc in os.listdir(folder):
		print("--> " + doc)

		id = doc.split(".txt")
		id = id[0]

		file = folder + doc
		with open(file, 'r', encoding='utf-8') as f:
			for line in f:
				save_tokens(id, line, rem_links = True, rem_mentions = True, rem_hashtags = True)

#-------------------------------------------------------------------------#
#Executa o metodo main
if __name__ == "__main__": main()
