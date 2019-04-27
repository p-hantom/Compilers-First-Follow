#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <stdio.h>
#define LOCAL
using namespace std;

bool isUpper(char x){
	if(x>='A'&&x<='Z'){
		return true;
	}
	return false;
}

int main(){
	#ifdef LOCAL
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	#endif
	multimap<char,string> production;
	map<char,set<char> > first,follow;
	char vn;
	string exp;

	//input & initialize
	cin>>vn>>exp;
	bool startSym=true;
	while(vn!='0'&&exp!="0"){
		if(first.find(vn)==first.end()){
			set<char> ch;
			first.insert(pair<char,set<char> >(vn,ch));
			if(startSym){
				ch.insert('#');
			}
			startSym=false;
			follow.insert(pair<char,set<char> >(vn,ch));
		}
		production.insert(pair<char,string>(vn,exp));
		cin>>vn>>exp;
	}

	//first
	bool isChanged = true;
	multimap<char,string>::iterator it;
	while(isChanged){
		isChanged=false;
		for(it=production.begin(); it!=production.end(); it++){
			bool insertE=true;
			string tmpStr=it->second;
			
			set<char> tmpSet=(first.find(it->first)->second);
			int beforeLen=tmpSet.size(),afterLen;
			for(int i=0;i<tmpStr.length();i++){
				if(!isUpper(tmpStr[i])){ //terminal
					tmpSet.insert(tmpStr[i]);
					
					insertE = false; //already inserted
					break;
				}
				else{  //non-terminal
					set<char> vnSet=(first.find(tmpStr[i])->second);
					vnSet.erase('^');
					tmpSet.insert(vnSet.begin(),vnSet.end());
					if(!(first.find(tmpStr[i])->second).count('^')){
						insertE = false;
						break;
					}
				}
			}
			if(insertE==true){
				tmpSet.insert('^');
			}
			afterLen=tmpSet.size();
			if(afterLen>beforeLen){
				isChanged=true;
				(first.find(it->first)->second)=tmpSet;
			}
		}
	}
	//output first
	map<char,set<char> >::iterator firstIt;
	set<char>::iterator firstSetIt;
	for(firstIt=first.begin();firstIt!=first.end();firstIt++){
		cout<<firstIt->first<<"  First:";
		for(firstSetIt=firstIt->second.begin();firstSetIt!=firstIt->second.end();firstSetIt++){
			cout<<*firstSetIt<<" ";
		}
		cout<<endl;
	}
	cout<<endl;

	//follow
	isChanged = true;
	
	while(isChanged){
		isChanged=false;
		for(it=production.begin();it!=production.end();it++){
			string tmpStr=it->second;
			// cout<<"tmpStr = "<<tmpStr<<endl;
			for(int i=0;i<tmpStr.length();i++){
				bool hasE=false;
				if(isUpper(tmpStr[i])){ //non-terminal
					
					// cout<<"now :"<<tmpStr[i]<<endl;
					set<char> tmpSet=(follow.find(tmpStr[i])->second);
					int beforeLen=tmpSet.size(),afterLen;
					if(i!=tmpStr.length()-1){ //not the last non-terminal
						
						if(!isUpper(tmpStr[i+1])){
							tmpSet.insert(tmpStr[i+1]);
							// cout<<"insert: "<<tmpStr[i+1]<<endl;
							// break;
						}
						else{
							hasE=true;
							for(int j=i+1;j<tmpStr.length();j++){
								if(!isUpper(tmpStr[j])){
									set<char> tmpSet1=follow.find(it->first)->second;
									tmpSet.insert(tmpStr[j]);
									hasE=false;
									break;
								}
								set<char> tmpSet1=first.find(tmpStr[j])->second;
								if(!tmpSet1.count('^')){
									hasE=false;
								}
								tmpSet1.erase('^');
								tmpSet.insert(tmpSet1.begin(),tmpSet1.end());
								if(hasE==false){
									break;
								}
								
							}
							
						}
					}
					if(i==tmpStr.length()-1 || hasE){ //the last non-terminal
						set<char> tmpSet1=follow.find(it->first)->second;
						tmpSet.insert(tmpSet1.begin(),tmpSet1.end());
					}
					afterLen=tmpSet.size();
					if(afterLen>beforeLen){
						isChanged=true;
						(follow.find(tmpStr[i])->second)=tmpSet;
					}
				}
			}
		}
	}
	//output follow
	map<char,set<char> >::iterator followIt;
	set<char>::iterator followSetIt;
	for(followIt=follow.begin();followIt!=follow.end();followIt++){
		cout<<followIt->first<<"  Follow: ";
		for(followSetIt=followIt->second.begin();followSetIt!=followIt->second.end();followSetIt++){
			cout<<*followSetIt<<" ";
		}
		cout<<endl;
	}

	// system("pause");
	
	return 0;
}
