//Array library
#include <iostream>
#include <string>

using namespace std;

//function declarations
int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target); 
int positionOfMax(const string a[], int n); 
int rotateLeft(string a[], int n, int pos); 
int countRuns(const string a[], int n); 
int flip(string a[], int n); 
int differ(const string a1[], int n1, const string a2[], int n2); 
int subsequence(const string a1[], int n1, const string a2[], int n2); 
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator); 

//helper functions
int my_min(int a, int b);

int main()
{	
	
	// string people[5] = { "hillary", "gary", "donald", "jill", "evan" };
	// cout << appendToAll(people, 5, "!!!") << endl;  // returns 5
 //    // now cand[0] is "hillary!!!", cand[1] is "gary!!!", ...,
 //    // and cand[4] is "evan!!!"

 //    cout << lookup(people, 5, "")<<endl;

 //    string cand[6] = { "jill", "hillary", "donald", "tim", "evan", "mike" };
	// int k = positionOfMax(cand, 6);   // returns 3, since  tim  is latest
 //    cout << k <<endl;                 	// in alphabetic order

	// string running[5] = { "evan", "donald", "gary", "jill", "hillary" };
	// int m = rotateLeft(running, 5, 1);  // returns 1
 //    // running now contains:  "evan", "gary", "jill", "hillary", "donald"
	// for(int i =0; i<5; i++) cout<<running[i];
	// cout << endl;
	// cout << m << endl;

	// string d[9] = {
 //    "tim", "ajamu", "mike", "mike", "donald", "donald", "donald", "mike", "mike"
	// };
	// int p = countRuns(d, 9);  //  returns 5
	// cout << p << endl;

	// string folks[6] = { "ajamu", "mike", "", "tim", "mindy", "bill" };
	// int q = flip(folks, 4);  // returns 4
 //    // folks now contains:  "tim"  ""  "mike"  "ajamu"  "mindy"  "bill"
	// for(int i =0; i < 6; i++) cout << folks[i] << " ";
	// 	cout << endl;
	// cout << q << endl;
	

	// string folks2[6] = { "ajamu", "mike", "", "tim", "mindy", "bill" };
	// string group[5] = { "ajamu", "mike", "bill", "", "tim" };
	// int r = differ(folks2, 6, group, 5);  //  returns 2
	// int s = differ(folks2, 2, group, 1);  //  returns 1
	// cout << r << endl;
	// cout << s << endl;

	// string names[10] = { "evan", "hillary", "mindy", "jill", "ajamu", "gary" };
	// string names1[10] = { "hillary", "mindy", "jill" };
	// int t = subsequence(names, 6, names1, 3);  // returns 1
	// cout<< t << endl;
	// string names2[10] = { "evan", "jill" };
	// int u = subsequence(names, 5, names2, 2);  // returns -1
	// cout << u << endl;

	string names[10] = { "evan", "hillary", "mindy", "jill", "ajamu", "gary" };
	string set1[10] = { "bill", "ajamu", "jill", "hillary" };
	int v = lookupAny(names, 6, set1, 4);  // returns 1 (a1 has "hillary" there)
	string set2[10] = { "tim", "donald" };
	int w = lookupAny(names, 6, set2, 2);  // returns -1 (a1 has none)
	cout<<v<<endl;
	cout << w << endl;

	string cand[6] = { "donald", "jill", "hillary", "tim", "evan", "bill" };
	int x = separate(cand, 6, "gary");  //  returns 3
	for(int i = 0; i<6; i++){
		cout << cand[i] << " ";
	}
	cout << endl << x <<endl;
	// cand must now be
	//      "donald"  "evan"  "bill"  "jill"  "tim"  "hillary"
	// or   "evan"  "bill"  "donald"  "hillary"  "jill"  "tim"
	// or one of several other orderings.
	// All elements < "gary" (i.e., "evan", "bill", and "donald")
	//   come before all others
	// All elements > "gary" (i.e., "tim", "jill", and "hillary")
	//   come after all others
	string cand2[4] = { "gary", "hillary", "jill", "donald" };
	int y = separate(cand2, 4, "hillary");  //  returns 2
	for(int i = 0; i<4; i++){
		cout << cand2[i] << " ";
	}
	cout <<endl << y <<endl;
	// cand2 must now be either
	//      "gary"  "donald"  "hillary"  "jill"
	// or   "donald"  "gary"  "hillary"  "jill"
	// All elements < "hillary" (i.e., "donald" and "gary") come
        // before all others.
	// All elements > "hillary" (i.e., "jill") come after all others.

}

int appendToAll(string a[], int n, string value){
	if(n<0) return -1;
	for(int i = 0; i<n; i++) a[i] += value;
	return n;
}

int lookup(const string a[], int n, string target){
	if(n<0) return -1;
	for(int i = 0; i <n; i++){
		if(a[i]==target) return i;
	}
	return -1;
}

int positionOfMax(const string a[], int n){
	if(n<=0) return -1;
	string currentHigh = a[0];
	int output = 0;
	for(int i = 1; i<n; i++){ // no need to cheeck first term on itself
		//set new currentHigh IFF a[i] is strictly HIGHER than
		if(a[i]>currentHigh) {
			currentHigh = a[i]; 
			output = i;
		}
	}
	return output;
}

int rotateLeft(string a[], int n, int pos){
	if(n<0 || pos <0 || pos >= n ) return -1; //pos can at most be n-1 (last position of array)
	string temp = a[pos];

	for(int i = pos; i < n-1; i++){
		a[i] = a[i+1];
	}
	a[n-1] = temp;
	return pos;
}

int countRuns(const string a[], int n){
	if(n<0) return -1;
	if(n==0) return 0;
	int output = 1;
	for(int i =1; i<n; i++){
		if(a[i] != a[i-1]) output++;
	}
	return output;
}

int flip(string a[], int n){
	if(n<0) return -1;
	for(int i = 0; i<n/2; i++){
		string temp = a[i];
		a[i] = a[n-1-i];
		a[n-1-i] = temp;
	}
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2){
	int min = my_min(n1,n2);
	if(min < 0) return -1;
	for(int i = 0; i<min; i++){
		if(a1[i]!=a2[i]) return i;
	}
	return min;
}
int my_min(int a, int b){
	if(a<b) return a;
	else return b;
}

int subsequence(const string a1[], int n1, const string a2[], int n2){
	if(my_min(n1,n2) <0 ) return -1;
	if(n2 == 0) return 0;

	for(int i = 0; i <= n1-n2; i++){ // <= to because index starts from 0 so in subtracting two 
		int j;						// array sizes we must make sure two arrays of same length
		for( j = 0; j<n2; j++){		// must be checked once, and length diff of 1 must be checked twice
			if(a1[i+j]!=a2[j]) break; //since we need to check the start pos of 0 and of 1, and so on for
		}								//larger differences
		if(j==n2) return i;
	}
	return -1; //for loop will not run if n2 > n1 implicitly, so returns -1 if n2 > n1 (and if subsequence not found)
}

int lookupAny(const string a1[], int n1, const string a2[], int n2){
	if(my_min(n1,n2) < 0) return -1;
	for(int a1Index = 0; a1Index<n1; a1Index++){
		for(int i = 0; i<n2; i++){
			if(a1[a1Index] == a2[i]) return a1Index;
		}
	}
	return -1;	
}

int separate(string a[], int n, string separator){
	if(n<0) return -1;
	int count = 0;
	//put the >seperator at the end
	for(int i = 0; i < n; i++){
		if(a[i]<separator) count++;
		else if(a[i]>separator){
			rotateLeft(a, n, i); //put >seperator strings to the end of manipulable array
			i--; //since next term is shifted, i needn't be incremented
			n--; //so last term of manipulable array is locked in position & no longer part of manipulable array
		}
	}
	//put the ==seperator in the middle (n now marks the end of the array containing those <=seperator)
	for(int i = 0; i<n; i++){
		if(a[i]==separator){
			rotateLeft(a,n,i);
			i--;
			n--;
		}
	}
	return count;
}