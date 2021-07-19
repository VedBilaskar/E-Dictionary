#include<bits/stdc++.h>
using namespace std;

class node
{
public:
	char data;
	node*left, *right, *mid;
	bool isWord;
	string meaning;
	node(char x) {data = x; left = right = mid = NULL; isWord = 0;}
};

class TST
{
	node*root;
	vector<vector<string>>dataset;
	void read_dataset()
	{
		string myText;
		ifstream MyReadFile("dsa.txt");
		while (getline (MyReadFile, myText))
		{
			vector <string> row;
			stringstream check1(myText);
			string intermediate;

			while (getline(check1, intermediate, ','))
				row.push_back(intermediate);


			dataset.push_back(row);

		}

		MyReadFile.close();
	}

	node* Insert(node * p, string&  word, string&  meaning, int index);
	node* search(node * p, string& word, int i);
	void traverse(node * p, string& out, vector < pair<string, string>>& words);
	void pre(node*p, int idx, string& prefix, vector<pair<string, string>>& suggestions);


public:
	TST()
	{
		read_dataset();

		root = NULL;

		for (auto v : dataset)
		{
			transform(v[0].begin(), v[0].end(), v[0].begin(), ::tolower);
			transform(v[1].begin(), v[1].end(), v[1].begin(), ::tolower);
			insert_word(v[0], v[1]);
		}


	}
	void insert_word(string& word, string& meaning);
	string get_meaning(string& word);
	void print_sorted_order();
	void auto_complete(string& prefix);


};


void TST::insert_word(string& word, string& meaning)
{
	root = Insert(root, word, meaning, 0);
}

string TST::get_meaning(string& word)
{
	transform(word.begin(), word.end(), word.begin(), ::tolower);

	node*t = search(root, word, 0);

	if (t)
		return t->meaning;

	return "word not found";

}

node* TST::Insert(node* p, string& word, string& meaning, int index)
{
	if (!p)
	{
		p = new node(word[index]);
		if (!root) root = p;
	}
	if (word[index] < p->data)
		p->left = Insert(p->left, word, meaning, index );
	else if (word[index] > p->data)
		p->right = Insert(p->right, word, meaning, index );
	else
	{
		if (index < word.length() - 1)
			p->mid = Insert(p->mid, word, meaning, index + 1);
		else
		{
			p->isWord = true;
			p->meaning = meaning;
		}

	}

	return p;
}


node*TST::search(node* p, string& word, int i)
{
	if (!p) return NULL;

	if (word[i] < p->data)
		return search(p->left, word, i);
	if (word[i] > p->data)
		return search(p->right, word, i);

	if (p->isWord && i == word.length() - 1)
		return p;

	else if (i == word.length() - 1)
		return NULL;

	return search(p->mid, word, i + 1);

}

void TST::traverse(node * p, string& out, vector < pair<string, string>>& words)
{
	if (p)
	{
		traverse(p->left, out, words);
		out.push_back(p->data);

		if (p->isWord)
		{
			words.push_back({out, p->meaning});
		}

		traverse(p->mid, out, words);
		out.pop_back();
		traverse(p->right, out, words);

	}
}

void TST::print_sorted_order()
{
	string out = "";
	vector < pair<string, string>> words;
	traverse(root, out, words);
	for (auto s : words)
	{
		cout << s.first << " -> " << s.second << endl;
	}
}

void TST::pre(node*root , int idx, string& prefix, vector<pair<string, string>>& suggestions)
{
	if (!root) return;

	if (prefix[idx] < root->data )
		pre(root->left, idx, prefix, suggestions);
	else if ( prefix[idx] > root->data)
		pre(root->right, idx, prefix, suggestions);
	else if (idx < prefix.length() - 1)
		pre(root->mid, idx + 1, prefix, suggestions);
	else
	{
		string out = "" ;
		traverse(root->mid, out, suggestions);
	}
}

void TST::auto_complete(string& prefix)
{
	transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);

	vector<pair<string, string>> suggestions;

	pre(root, 0, prefix, suggestions);

	if (suggestions.empty())
	{
		cout << "no matches found\n";
		return;
	}

	for (auto s : suggestions)
	{
		cout << prefix << s.first << " -> " << s.second << endl;
	}

}


int main()
{

	TST my_dict;


	cout << "\t********E-DICTIONARY*********\n";
	cout << "\tquery1-> get meaning\n";
	cout << "\tquery2-> print words in a sorted order\n";
	cout << "\tquery3-> get a list of suggested words with meanings based on a prefix\n";



	int t;
	cin >> t;

	while (t--)
	{
		int q;
		cin >> q;
		switch (q)
		{
		case 1: {
			string str;
			cin >> str;
			cout << str << " means " << my_dict.get_meaning(str);
			break;
		}
		case 2: {
			my_dict.print_sorted_order();
			cout << endl;
			break;
		}
		case 3: {
			string pre;
			cin >> pre;
			my_dict.auto_complete(pre);
			cout << endl;
			break;
		}
		default: break;

		};
	}






	return 0;
}