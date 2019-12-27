/* KnapsackSolver ver 1.0.2
   Created by: Ηλίας Λάμπρου
   Δημιουργήθηκε: 15/12/2019
   Τροποποιήθηκε: 27/12/2019
   
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <chrono>
#include <algorithm>
#include <limits>
#include <queue>
#include <thread>
#include <direct.h>


//using namespace std;
//using namespace std::chrono;

// Η struct item περιέχει τα στοιχεία κάθε αντικειμένου 
struct item
{
    int id;         // αριθμός αντικειμένου με τη σειρά που βρίσκεται στο αρχείο
    int profit;     // αξία
    int weight;     // βάρος
    int bound;
    int level;
    std::vector<int> itemList;
};

long knapsack_time_out = 10000;
// Ορίζουμε ένα struct το οποίο θα αποθηκεύει σε μια λίστα με τα επιμέρους στοιχεία 
// των αντικειμένων και τη συνολική χωρικότητα του σακιδίου. 
// Για την περίπτωση που ο υπολογισμός μπορεί να διαρκέσει πολύ ορίζουμε και ένα χρόνο διακοπής της διαδικασίας
struct knapsack_problem{
    std::vector<item> items;     // Λίστα με τα συνολικά αντικείμενα 
    int capacity;                   // Χωρητικότητα σακιδίου
};

unsigned long lastSolutionTime = 0;
/*============================================================= read_data
  =============================================================
   Η συνάρτηση read_data δέχεται ως παράμετρο το όνομα ενός αρχείου με δεδομένα knapsack και
   επιστρέφει το περιεχόμενο σε μορφή (struct) knapsack_problem
 */
knapsack_problem read_data(std::string& fn) {
    std::ifstream fin(fn);     // Προσπαθούμε να ανοίξουμε το αρχείο
    if (!fin.good()) {     // Ελέγχουμε αν δεν έχει ανοιχθεί
        std::cerr << "Error opening file " << fn << std::endl;   // εκτυπώνουμε μήνυμα λάθους σε αυτή την περίπτωση
        system("pause");
        exit(-1);         
    }

    knapsack_problem ks;  // Δημιουργούμε μια μεταβλητή τύπου knapsack_problem 
                          // σε αυτή θα αποθηkευτούν τα στοιχεία του αρχείου knapsack
    int items_number;     // Ορίζουμε μια μεταβλητή items_number για να αποθηκεύουμε τον 
                          // αριθμό των αντικειμένων που βρίσκονται στο αρχείο
    fin >> items_number;  // Διαβάζουμε από την πρώτη γραμμή τον αριθμό των αντικειμένων και τον κρατάμε στην items_number
    for (int i = 0; i < items_number; i++)  // Για όσα είναι τα αντικείμενα του αρχείου θα τρέξουμε τον παρακάτω κώδικα
                          // για να διαβάσουμε τα στοιχεία κάθε αντικειμένου 
    {
        item an_item;     // Η μεταβλητή αυτή κρατάει κάθε φορά τα στοιχεία κάθε αντικειμένου από το αρχείο
        fin >> an_item.id;      // Διαβάζουμε το id (σειρά του αντικειμένου στο αρχείο)
        fin >> an_item.profit;  // Διαβάζουμε την αξία και την αποθηκεύουμε στην an_item.profit
        fin >> an_item.weight;  // Διαβάζουμε το βάρος και το αποθηκεύουμε στο an_item.weight
        ks.items.push_back(an_item);  // Εισάγουμε το an_item στο τέλος της λίστας του ks (knapsack_problem)
    }
    fin >> ks.capacity;         // Τέλος διαβάζουμε την χωρητικότητα του σακιδίου και την αποθηκεύουμε στο ks.capacity
    return ks;  // επιστρέφουμε μια δομή τύπου knapsack_problem η οποία περιέχει όλα τα στoιχεία του συγκεκριμένου προβλήματος
}

/*============================================================= print_knapsack_problem_info
  =============================================================
   Η print_knapsack_problem_info δέχεται ως παράμετρο ένα knapsack πρόβλημα ως αναφορά σε δομή knapsack_problem
   και εκτυπώνει τα στοιχεία του στην οθόνη
 */
void print_knapsack_problem_info(knapsack_problem& ks)
{
    std::cout << "Items=" << ks.items.size() << "   (id, Profit, Weight)\n";  // εκτύπωση αριθμού αντικειμένων του προβλήματος
    for (int i = 0; i < ks.items.size(); i++)               // εκτύπωση όλων στοιχείων των αντικειμένων του προβλήματος με την σειρά
                                                            // που βρίσκονται στο αρχείο
    {                                                       // Εκτυπώνεται η σειρά (id) η αξία (profit) και το βάρος (weight) καθε ενός με τη σειρά 
        std::cout << ks.items[i].id << "  " << ks.items[i].profit << "  " << ks.items[i].weight << std::endl;
    }
    std::cout << "Capacity=" << ks.capacity << std::endl;   // Τέλος εκτυπώνεται η χωρητικότητα του σακιδίου (capacity)
}

//================================================================
bool cmp(struct item a, struct item b)
{
    double r1 = (double)a.profit / a.weight;
    double r2 = (double)b.profit / b.weight;
    return r1 > r2;
}


/*=============================================================
  ==================  1 Greedy Approach  ======================
  =============================================================

 */

 //============================================= greedy_approach
std::vector <item> greedy_approach(knapsack_problem& ks){

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "--- Greedy approach\n";
    // Ταξινομούμε τον πίνακα σύμφωνα με την συνάρτηση ταξινόμισης cmp (ο μεγαλύτερος λόγος αξίας/βάρος πρώτα )
    sort(ks.items.begin(), ks.items.end(), cmp);

    int curWeight = 0;              // Τρέχον συνολικό βάρος κατα τον υπολογισμό
    double finalvalue = 0.0;        // Ποσοστό επιτυχίας: συνολικό βάρος αντικειμένων του σακιδίου / χωρητικότητα
    std::vector<item> solutionList;

    // Looping through all Items 
    for (int i = 0; i < ks.items.size(); i++) {
        item tempItem = ks.items.at(i);
        // If adding Item won't overflow, add it completely 
        if (curWeight + tempItem.weight <= ks.capacity)
        {
            solutionList.push_back(tempItem);
            curWeight += tempItem.weight;
            finalvalue += tempItem.profit;
        }

        // If we can't add current Item, add fractional part of it 
        else
        {
            //int remain = ks.capacity - curWeight;
            //finalvalue += tempItem.profit * ((double)remain / tempItem.weight);
            break;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        lastSolutionTime = duration.count();
        if (lastSolutionTime > knapsack_time_out) {
            std::cout << "Time out: t>" << knapsack_time_out << "\n";
            return solutionList;
        }
    }

    std::cout << "maxProfit=" << finalvalue << "\n";
    std::cout << "maxWeight=" << curWeight << "\n";
    std::cout << "time=" << lastSolutionTime << "\n";
    return solutionList;
}


/*=============================================================
  ================ 2  Βrute Force Solver ======================
  =============================================================

   Η brute_force_solver δέχεται ως παράμετρο ένα knapsack πρόβλημα
 */
 //-- tool: get_profit1
int get_profit(knapsack_problem& ks, std::vector<item>& sol){
    int total_profit = 0;
    int total_weight = 0;
    for (item an_item : sol)
    {
        total_weight += an_item.weight;
        if (total_weight > ks.capacity)
            return -1;
        total_profit += an_item.profit;
    }
    return total_profit;
}
//================================================================= brute_force_solver
std::vector <item> brute_force_solver(knapsack_problem& ks) {
    //std::cout<<"=======================================\n";
   /// std::cout<<"=== 2 Solution by: brute force algorithm\n";
   // std::cout<<"=======================================\n";
    //std::chrono::high_resolution_clock::time_point t1=std::chrono::high_resolution_clock::now();
    std::cout << "--- Brute force\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<item> result;
    int max_profit = -1;
    int n = ks.items.size();
    int total = 1 << n;          // με βάση τον αριθμό των αντικειμένων βρίσκουμε τον αριθμό των συνδιασμών
    if (n >= 31)  total = std::numeric_limits<int>::max(); // εάν ο αριθμός των συνδιασμών είναι μεγαλύτερος από τη μέγιστη τιμή του int θα ελέγξουμε όσα αντικείμενα έχει η max int
    std::vector<item> sol;
    for (int i = 0;i < total;i++){    //  Tα bits του i που θα είναι 1 θα περιέχουν τους συνδιασμούς τους αντικειμένων αντίστοιχα με την θέση τους στο 32bit αριθμό
        sol.clear();
        for (int j = 0;j < n;j++){    //  θα ελέγξουμε όλα τα bits του i (32bit) 
            if ((i >> j) & 1)         //  ολισθαίνουμε το i δεξιά ώστε να μπορούμε να συγκρίνουμε το τελευταίο κάθε φορά bit με το ένα.
                sol.push_back(ks.items[j]);  // εάν το αντοίστοιχο στη θέση j bit του i είναι ένα εισάγουμε το αντίστοιχο αντικείμενο στη λίστα

        }
        int profit = get_profit(ks, sol);
        if (profit > max_profit)
        {
            max_profit = profit;
            result = sol;

        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        lastSolutionTime = duration.count();
        if (lastSolutionTime > knapsack_time_out) {
            std::cout << "Time out: t>" << knapsack_time_out << "\n";
            return result;
        }
    }

    std::cout << "maxProfit=" << max_profit<< "\n";
    int maxWeight = 0;
    for (auto myItem : result) maxWeight += myItem.weight;
    std::cout << "maxWeight=" << maxWeight << "\n";
    std::cout << "time=" << lastSolutionTime << "\n";

       return result;

}

/*=================================================================
  ================== 3 Branch and Bound============================
  =================================================================
*/
//=================================================
int bound(item u, int n, int W, item arr[]){
    if (u.weight >= W) return 0;
    int profit_bound = u.profit;
    int j = u.level + 1;
    int totweight = u.weight;

    while ((j < n) && (totweight + arr[j].weight <= W)) {
        totweight += arr[j].weight;
        profit_bound += arr[j].profit;
        j++;
    }
    if (j < n) profit_bound += (W - totweight) * arr[j].profit / arr[j].weight;
    return profit_bound;
}

//=========================================================== branch_and_bound

std::vector <int>  branch_and_bound_action2(int W, item arr[], int n) {
    std::cout << "=============branch_and_bound_action2 =======================\n";
    std::sort(arr, arr + n, cmp);
    std::queue <item> Q;
    item u, v;
     auto start = std::chrono::high_resolution_clock::now();
    u.level = -1;
    u.profit = u.weight = 0;
    Q.push(u);
    int maxProfit = 0;
    int maxWeight = 0;
    std::vector<int> selectedItems;

    while (!Q.empty()) {        // Η ουρά περιέχει τα αντικείμενα που πρ΄πει να ελεγθούν. Εφόσον δεν είναι άδεια συνεχίζουμε
        u = Q.front();          // Κρατάμε στη u το πρώτο αντικείμενο από την ουρά
        Q.pop();                // και μετά το αφαιρούμε αφού θα ελεγθεί παρακάτω 
        if (u.level == -1) v.level = 0;    // Την πρώτη φορά που θα εκτελεσθεί το loop η u.level είναι -1 και την κάνουμε 0
        if (u.level == n - 1) continue;    // εάν φτάσουμε στο τελευταίο αντικείμενο σταματά με και πάμε να εξετάσουμε το επόμενο της ουράς

        v.level = u.level + 1;  // Ο έλεγχος θα γίνει στο επόμενο level (αντικείμενο)

        v.weight = u.weight + arr[v.level].weight;    //αποθηκεύουμε στη v το επόμενο αντικείμενο και προσθέτουμε το μέχρι τώρα βάρος και αξία
        v.profit = u.profit + arr[v.level].profit;
        v.id = arr[v.level].id;
        v.itemList.clear();                           // ίσως τζάμπα να το έβαλα αυτό. Να το ξαναδω στο τέλος
        v.itemList = u.itemList;
        v.itemList.push_back(v.id);

        if (v.weight <= W && v.profit > maxProfit) {  // εάν το βάρος δεν έχει ξεπεράσει το μέγιστο βάρος και η αξία του δεν είναι μικρότερη από αυτή που μέχρι τώρα έχουμε 
            maxProfit = v.profit;                     // επιλέγουμε το αντικείμενο κρατώντας το ως καλύτερη μέχρι τώρα επιλογή για το βάρος και την αξία
            maxWeight = v.weight;
            selectedItems = v.itemList;               // κρατάμε και τη λίστα με τα επιλεγμένα αντικείμενα του συνδιασμού ως η καλύτερη μέχρι τώρα
        }

        v.bound = bound(v, n, W, arr);                // υπολογίζουμε την μέγιστη αξία για όσα αντικείμενα ακολουθούν

        if (v.bound > maxProfit) {                    // αν η μέγιστη αξία είναι καλύτερη από αυτή που έχουμε
            Q.push(v);                                // θα πρέπει να ελεγθεί ο κλάδος του v
        }
        v.weight = u.weight;                          // τώρα θα εισάγουμε ξανά το αντικείμενο στην ουρά για έλεγχο αλλά αυτή την φορά δεν θα συμπεριλάβουμε την δική του αξία και βάρος  
        v.profit = u.profit;                          // οι λύσεις που θα δημιουργηθούν δεν θα συμπεριλαμβάνουν το τρέχον αντικείμενο  
        v.id = u.id;
        v.itemList = u.itemList;    
        v.bound = bound(v, n, W, arr);                // υπολογίζουμε ξανά την μέγιστη δυνατή αξία για τα αντικείμενα που ακολουθούν  
        if (v.bound > maxProfit) Q.push(v);           // εάν υπάρχει περίπτωση καλύτερης λύσης εισάγουμε το αντικείμενο στην ουρά για έλεγχο

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        lastSolutionTime = duration.count();
        if (lastSolutionTime > knapsack_time_out) {
            std::cout << "\nTime out: t>" << knapsack_time_out << "\n";
            return selectedItems;
        }

    }

    std::cout << "maxProfit=" << maxProfit << "\n";
    std::cout << "maxWeight=" << maxWeight << "\n";
    std::cout << "time=" << lastSolutionTime << "\n";
    return selectedItems;
}
//=============================================================
std::vector <item>  branch_and_bound(knapsack_problem& ks) {
 
    int n = ks.items.size();
    item *arr = new item[n];
    for (int i = 0;i < n;i++) arr[i] = ks.items.at(i);
    std::cout << "--- Branch and bound\n";
    std::vector <int> solutionIDs = branch_and_bound_action2(ks.capacity, arr, n);
    
    //std::cout << "solutionIDs size=" << solutionIDs.size() << "\n";
    std::vector <item> solution;
    for (int i = 0;i < solutionIDs.size();i++) {       // Δημιουργούμε μια λίστα με τα αντικείμενα που επιλέχθηκαν στη λύση
        int itemIndex = solutionIDs[i]-1;
        //std::cout << itemIndex << "\n";
        //std::cout << itemIndex << ks.items[itemIndex].id<<" " << ks.items[itemIndex].profit<<" " << ks.items[itemIndex].weight<<"\n";
        solution.push_back(ks.items[itemIndex]);
    }

    delete[] arr;

    return solution;
}

/*===========================================================
  =================4 Dynamic Algorithm ======================
  ===========================================================
*/
int max(int x, int y) {
    return (x > y) ? x : y;
}
std::vector <item> dynamicAlgorithm(knapsack_problem& ks) {
    std::cout << "--- Dynamic\n";
    auto start = std::chrono::high_resolution_clock::now();
    bool isTimeout = false;

    int W = ks.capacity;                      // αποθηκεύουμε την χωρητικότητα του σακιδίου στην W 
    int n = ks.items.size();                   // αποθηκεύουμε τoν αριθμό των αντικειμένων στο n
    std::vector<int> w(n, 0);   // create a vector to hold "size" int's
    std::vector<int> v(n, 0);   // create a vector to hold "size" int's
    int m = 0;
    for (auto& myItem : ks.items) {
        w[m] = myItem.weight;// ks.items[i].weight;
        v[m] = myItem.profit;//ks.items[i].profit;
        m++;
    }
    int i, wt;
    int n_ = n + 1;
    int W_ = W + 1;
    
    std::vector<std::vector <int> > K(n_,std::vector<int>(W_,0));
    for (i = 0; i <= n; i++) {                 // τρέχουμε όλες τις γραμμές του πίνακα Κ και ταυτόχρονα όλα τα αντικείμενα (+ 1 στοιχείο ακόμη στο τέλος) 

        for (wt = 0; wt <= W; wt++) {           // τρέχουμε όλες τις στήλες 
            if (i == 0 || wt == 0) K[i][wt] = 0; // κάνουμε 0 την πρώτη γραμμή και στήλη του πίνακα
            else if (w[(i - 1)] <= wt)             // εάν το βάρος του προηγούμενου αντικειμένου είναι μικρότερο ή ίσο με τον αριθμό της στήλης
                K[i][wt] = max(v[(i - 1)] + K[i - 1][wt - w[(i - 1)]], K[i - 1][wt]); // υπολογίζουμε από την σχέση την τιμή του στοιχείου  του πίνακα
            else
                K[i][wt] = K[i - 1][wt];              // διαφορετικά το κάνουμε ίσο με το στοιχείο της προηγούμενης γραμμής της ίδια στήλης
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        lastSolutionTime = duration.count();
        if (lastSolutionTime > knapsack_time_out) {
            std::cout << "Time out: t>" << knapsack_time_out << "\n";
            isTimeout = true;
            break;
        }


    }
  
    std::vector <item> solution;                    // δημιουργούμε το vector που θα περιέχει τα επιλεγμένα στη λύση αντικείμενα
    if (!isTimeout) {
        while (n != 0) {                            // το n περιέχει την αριθμό των αντικειμένων και στον πίνακα δείχνει στην τελευταία γραμμή
                                                    // τρέχουμε όλα τα στοιχεία του πίνακα (αντίστροφα) από την τελευταία στήλη και γραμμή προς την αρχή του     
            if (K[n][W] != K[n - 1][W]) {           // ελέγχουμε αν το τρέχον στοιχείο του πίνακα είναι διαφορετικό από το προηγούμενο
                                                    // σε περίπτωση που είναι, το αντικείμενο αποτελεί μέρος της λύσης 
                item sol = ks.items.at((n - 1));    // δημιουργούμε ένα struct sol για να αποθηκεύουμε το αντικείμενο που έχει επιλεχθεί στη λύση
                solution.push_back(sol);            // προσθέτουμε το αντικείμενο στη λίστα με τα επιλεγμένα αντικείναμε
                W = W - w[(n - 1)];                 // συνεχίζουμε τον έλεγχο από την προηγούμενη στήλη. Δηλαδή κάθε φορά που βρίσκουμε ένα μέρος της λύσης
                                                    // αλλάζουμε στήλη πηγαίνοντας μία πίσω
            }
            n--;                                    // ενώ αν δεν βρούμε μέρος της λύσης αλλάζουμε γραμμή πηγαίνοντας μία γραμμή πίσω μέχρι να ελέγξουμε και
                                                    // και την δεύτερη γραμμή του πίνακα (η πρώτη είναι μηδενισμένη)
        }
    
    }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        lastSolutionTime = duration.count();
   

    for (int i = 0; i < n + 1; ++i) {           // αποδέσμευση μνήμης για όλα τα vectors
        K[i].clear();
    }
    K.clear();
    v.clear();
    w.clear();
    return solution;                            // επιστρέφουμε το vector solution το οποίο περιέχει τη λιστα με τα επιλεγμένα στη λύση αντικείμενα
}


/*============================================================= print_solution
  =============================================================
   Η print_solution δέχεται ως παράμετρο την λύση ενός προβλήματος knapsack_problem
   και εκτυπώνει τα στοιχεία του
 */
void print_solution(std::vector<item> sol) {
    std::cout << "------------- Solution ------------\n";
    std::string output;
    int total_weight = 0;
    int total_profit = 0;
    char buffer[100];
    for (int i = 0;i < sol.size();i++) {
        std::cout << sol[i].id << " " << sol[i].profit << " " << sol[i].weight << "\n";
        total_weight += sol[i].weight;
        total_profit += sol[i].profit;
    }
    std::cout << "Total profit:" << total_profit << "\n";
    std::cout << "Total weight:" << total_weight << "\n";
 
}
/*============================================================= export_solution
  =============================================================
   Η export_solution  αποθηκεύει τη λύση ενός knapsach προβλήματος σε ένα αρχείο
 */
void export_solution(knapsack_problem& ks, std::vector<item> sol, std::string& fn, int alg){
    
    std::fstream fout;
    fout.open(fn, std::ios::out);
    if (!fout.good()) {
        std::cerr << "Error accesing file" << fn << std::endl;
        exit(-1);
    }
    std::string output;
    switch (alg) {
        case 1:fout << "---------- Greedy approach ----------\n";break;
        case 2:fout << "---------- Brute force --------------\n";break;
        case 3:fout << "---------- Branch and bound ---------\n";break;
        case 4:fout << "---------- Dynamic programming ------\n";break;
    }
    int total_weight = 0;
    int total_profit = 0;
    for (int i = 0;i < sol.size();i++)
    {
        fout << sol[i].id<<" "<<sol[i].profit<<" "<< sol[i].weight<<"\n";
        total_weight += sol[i].weight;
        total_profit += sol[i].profit;
    }
    fout << " ";
    fout << total_profit;
    fout << " ";
    fout << total_weight;
    fout << "\n";

    fout.close();
}


//============================================================
//============================================================
void calculateProfitWeight(std::vector<item>& solution, std::fstream& fout) {
    int w = 0;
    int v = 0;
    for (int i = 0; i < solution.size();i++) {
        item myItem = solution.at(i);
        w += myItem.weight;
        v += myItem.profit;
    }
    std::string s = ";"+ std::to_string(w) + ";"+std::to_string(v);
    fout << s;
}

//============================================================ delay
//============================================================
void delay(int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

//============================================================ readTimeOut
  
 long readTimeOut() {
     std::ifstream fin("knapsackTimeOut.txt");
     if (!fin.good()) return 10000;
     long t;
     fin >> t;
    return t; 
}
 //============================================================ saveTimeOut
 void saveTimeOut(long theTime) {
     std::fstream fout;
     fout.open("knapsackTimeOut.txt", std::ios::out);
     if (!fout.good()) return;
     fout << theTime;
     fout.close();
 }
 //============================================================ saveTimeOut
 void enterNewTimeOut(){
     system("CLS");
     std::cout << "Current timeout: " << knapsack_time_out << "\n";
     std::cout << "Enter new timeout: ";
     std::fstream fout;
     fout.open("knapsackTimeOut.txt", std::ios::out);
     if (!fout.good()) return;
     int t=-1;
     std::cin.clear();
     std::cin >> t;
     std::cout << "timeout=" << t;
     if (t>=0) {
         knapsack_time_out = t;
         saveTimeOut(knapsack_time_out);
      }
     
 }
 
 
/*============================================================ createAllProblemFiles
  Δημιουργεί όλα τα απαραίτητα για την εργασία προβλήματα knapsack 
  Δημιουργεί τα ονόματα των αρχείων και καλεί κάθε φορά την εξωτερική γεννήτρια για να παραχθεί το αρχείο
*/
void createAllProblemFiles(){
  int n[]={10,50,100,500};
  int r[]={50,100,500,1000};
  int type[]={1,2,3,4} ;
  std::string filename;
  std::string filenameWithArguments;
  char generatorFilename[]= "knapsack_gen.exe";
  char generatorOutputFilename[]= "problem_.txt";
  std::cout << "==================================\n";
  std::cout << "Creating Knapsack files...\n";
  
  int status= _mkdir("knapsack_problems");
  std::cout << "status="<<status<<"\n";
  //if ((status < 0) && (errno != EEXIST)) {}

  int fileCount=0;
  for (int k1=0;k1<4;k1++) {
    for (int k2=0;k2<4;k2++) {
        for (int k3=0;k3<4;k3++) {
            for (int k4=1;k4<=5;k4++) {
               int n_=n[k1];
               int r_=r[k2];
               int type_=type[k3];
               int i=k4;
               int S=5;
               filename = "knapsack_problems/problem_" + std::to_string(n_) + "_" + std::to_string(r_) + "_" + std::to_string(type_) + "_" + std::to_string(i) + "_5.txt";
               fileCount++;
               filenameWithArguments = "knapsack_gen.exe " + std::to_string(n_) + " " + std::to_string(r_) + " " + std::to_string(type_) + " " + std::to_string(i) + " " + std::to_string(S);
               std::cout << filenameWithArguments << "\n";
               int retCode = system(filenameWithArguments.c_str());
               int value = rename(generatorOutputFilename, filename.c_str());
               if(!value) printf("File name changed successfully\n");
               else {
                    perror("Error");
                    exit(0);
               }
            }
        }
    }
  }
  std::cout << "Finished/n";
  std::cout<<"Created files: "<<fileCount<<"\n";
  
}
/*============================================================= runAll
  =============================================================
   Η runAll βρίσκει τις λύσεις με όλους τους δυνατούς αλγόριθμους και εξάγει το αποτέλεσμα σε αρχείο CSV
   Δημιουργεί το αρχείο _results.csv και καθώς τρέχει ο κάθε αλγόριθμος προσθέτει την δική του λύση μέσα στο αρχείο
 */
void runAll(int selection) {
    int n[] = { 10,50,100,500 };
    int r[] = { 50,100,500,1000 };
    int type[] = { 1,2,3,4 };
    char filename[50];
    std::string filename_string;
    int fileCount = 0;
    std::string csv_path = "_results.csv";
    std::remove(csv_path.c_str());
    std::fstream fout;
    std::cout << "----- csv_path=" << csv_path << "\n";
    fout.open(csv_path, std::ios::out);
    if (!fout.good()) {     // Ελέγχουμε αν δεν έχει δημιουργηθεί
        std::cerr << "--- Error opening file " << csv_path << std::endl;   // εκτυπώνουμε μήνυμα λάθους σε αυτή την περίπτωση
        system("pause");
        exit(-1);        
    }
    std::string header = "INSTANCE";
    if ((selection == 0) || (selection == 1)) header.append(";WEIGHT_GR;VALUE_GR;EXECUTION_TIME_GR");
    if ((selection == 0) || (selection == 2)) header.append(";WEIGHT_BF;VALUE_BF;EXECUTION_TIME_BF");
    if ((selection == 0) || (selection == 3)) header.append(";WEIGHT_BB;VALUE_BB;EXECUTION_TIME_BB");
    if ((selection == 0) || (selection == 4)) header.append(";WEIGHT_DP;VALUE_DP;EXECUTION_TIME_DP");
    if ((selection == 0) || (selection == 5)) header.append(";WEIGHT_OT;VALUE_OT;EXECUTION_TIME_IP");
    if ((selection == 0) || (selection == 6)) header.append(";WEIGHT_IP;VALUE_IP;EXECUTION_TIME_OT");

    header.append("\n");
    fout << header;


    for (int k1 = 0;k1 < 4;k1++) {
        for (int k2 = 0;k2 < 4;k2++) {
            for (int k3 = 0;k3 < 4;k3++) {
                for (int k4 = 1;k4 <= 5;k4++) {
                    int n_ = n[k1];
                    int r_ = r[k2];
                    int type_ = type[k3];
                    int i = k4;
                    int S = 5;
                    std::vector<item> result;
                    
                    filename_string = "problem_" + std::to_string(n_)+"_"+ std::to_string(r_) + "_" + std::to_string(type_) + "_" + std::to_string(i) + "_5.txt"  ;
                    fileCount++;
                    std::cout << "------------------"<< fileCount<<"-------------------\n";
                    std::cout << "filename= " << filename_string << "\n";
                    fout << ";" << filename_string;
                    filename_string = "knapsack_problems/" + filename_string;
                    knapsack_problem ks = read_data(filename_string);
                    std::cout << "### ks.capacity=" << ks.capacity << "\n";
                    std::cout << "### ks.size=" << ks.items.size() << "\n";

                    // print_knapsack_problem_info(ks);

                    //----  greedy_approach ---------
                    if ((selection == 0) || (selection == 1)) {
                        result.clear();
                        result = greedy_approach(ks);
                        calculateProfitWeight(result, fout);
                        fout << ";" << lastSolutionTime;
                    }

                    //----  Brute force ---------
                    if ((selection == 0) || (selection == 2)) {
                        result.clear();
                        result = brute_force_solver(ks);
                        calculateProfitWeight(result, fout);
                        fout << ";" << lastSolutionTime;
                      }

                    //----  Branch and bound ---------
                    if ((selection == 0) || (selection == 3)) {
                        result.clear();
                        result = branch_and_bound(ks);
                        calculateProfitWeight(result , fout);

                        //fout << ";" << solution.maxWeight << ";" << solution.maxProfit;
                        fout << ";" << lastSolutionTime;

                    }

                    //----  Dynamic algorithm ---------
                    if ((selection == 0) || (selection == 4)) {
                        result.clear();
                        result = dynamicAlgorithm(ks);
                        calculateProfitWeight(result, fout);
                        fout << ";" << lastSolutionTime;
                    }

                    //----  Dynamic OR-Tools solver---------
                    if ((selection == 0) || (selection == 5)) {
                        fout.close();
                        std::string filenameWithArguments = "or_tools_dynamic.exe " + filename_string + " " + csv_path;
                        std::cout << "Filename: " << filenameWithArguments << "\n";
                        int retCode = system(filenameWithArguments.c_str());
                        fout.open(csv_path, std::ios::app);
                    }


                    //----  Integer OR-Tools solver ---------
                    if ((selection == 0) || (selection == 6)) {
                        fout.close();
                        std::string filenameWithArguments = "or_tools_integer.exe " + filename_string + " " + csv_path;
                        std::cout << "Filename: " << filenameWithArguments << "\n";
                        int retCode = system(filenameWithArguments.c_str());
                        fout.open(csv_path, std::ios::app);
                    }

                    fout << "\n";
                }
            }
        }
    }

    fout.close();
    std::cout << "Files: " << fileCount << "\n";
}

/*=============================================================== printOneProblemSolution
  Η ρουτίνα ζητάει από τον χρήστη να εισάγει 5 αριθμούς
  Δημιουργεί ένα αρχείο προβλήματος knapsack καλώντας μια εξωτερική γεννήτρια με τις κατάλληλες παραμέτρους
  Εκτυπώνει τα στοιχεία του προβλήματος στην οθόνη
  Ζητάει από τον χρήστη να επιλέξει αν επιθυμεί να γίνει επίλυση με κάποιον αλγόριθμο
  Αποθηκεύει την λύση σε αρχείο πχ solution_10_50_1_2_5_a2.txt  όπου οι τελευταίοι χαρακτήρες του ονόματος εξαρτώται από τον αλγόριθμο επίλυσης
  και μπορεί να είναι από a1 έως a6
  Η λύση περιέχει και μια λίστα με τα επιλεγμένα αντικείμενα
*/
void printOneProblemSolution() {
    system("CLS");
    std::cout << "\nCreate knapsack problem. Enter 5 numbers: ";
    int n;
    int r;
    int t;
    int i;
    int s;
    std::cin.clear();
    std::cin >> n;
    std::cin >> r;
    std::cin >> t;
    std::cin >> i;
    std::cin >> s;

    std::string  fn = "problem_" + std::to_string(n) + "_" + std::to_string(r) + "_" + std::to_string(t) + "_" + std::to_string(i) + "_" + std::to_string(s) + ".txt";
    std::string  genfn = "problem_.txt";   // Generator output filename
    
    
    std::cout << "\nProblem filename: " << fn << "\n";
    std::string generatorFilename = "knapsack_gen.exe " + std::to_string(n) + " " + std::to_string(r) + " " + std::to_string(t) + " " + std::to_string(i) + " " + std::to_string(s);
    //std::cout << "generatorFilename=" << fn << "\n";
    int retCode = system(generatorFilename.c_str());
    int value = rename(genfn.c_str(),fn.c_str());
    knapsack_problem ks = read_data(fn);
    std::cout << "\n===========Problem info ===========\n";
    //std::cout << "----- Capacity=" << ks.capacity << "\n";
    
    print_knapsack_problem_info(ks);
    
    std::cout <<"\n";
    std::cout << "========================================\n";
    std::cout << "Select algorithm to solve this problem\n";
    std::cout << "--------------------------------------\n";
    std::cout << " 1 Greedy approach\n";
    std::cout << " 2 Brute force\n";
    std::cout << " 3 Branch and bound\n";
    std::cout << " 4 Dynamic programming\n";
    std::cout << " 5 Dynamic OR-Tools solver\n";
    std::cout << " 6 Integer OR-Tools solver\n";
    std::cout << " 7 Exit without print solution\n";
    std::cout << "--------------------------------------\n";
    std::cout << " Enter your choice >";
    
    std::cin.clear();
    int selection = 0;
    std::cin >> selection;
    
    if ((selection < 0) || (selection > 6)) exit(0);
    std::string solution_fn = "solution_" + std::to_string(n) + "_" + std::to_string(r) + "_" + std::to_string(t) + "_" + std::to_string(i) + "_" + std::to_string(s) + "_a"+ std::to_string(selection)+".txt";

    std::vector<item> result;
    std::string filenameWithArguments;
    switch (selection) {
        case 1: result = greedy_approach(ks);
                print_solution(result);
                export_solution(ks, result, solution_fn,1);
            break;
        case 2: result = brute_force_solver(ks);
                print_solution(result);
                export_solution(ks, result, solution_fn,2);
            break;
        case 3: result = branch_and_bound(ks);
                print_solution(result);
                export_solution(ks, result, solution_fn, 2);
            break;
        case 4: result = dynamicAlgorithm(ks);
                print_solution(result);
                export_solution(ks, result, solution_fn,4);
            break;
        case 5: filenameWithArguments = "or_tools_dynamic.exe " + fn + " " + solution_fn+" 1";// Η τελική παράμετρος 1 έχει ως αποτέλεσμα το εξωτερικό αρχείο μην προσθέσει τις
                //std::cout << "Filename: " << filenameWithArguments << "\n";                 // πληροφορίες στο τρέχον CSV αρχείο. Αλλά να δημιουργήσει ένα καινούργιο αρχείο με την λύση αντίστοιχο   
                system(filenameWithArguments.c_str());                                        // με αυτό που δημιουγεί η print_solution.
                break;
        case 6: filenameWithArguments = "or_tools_integer.exe " + fn + " " + solution_fn + " 1";
                //std::cout << "Filename: " << filenameWithArguments << "\n";
                system(filenameWithArguments.c_str());
            break;
    default: break;
    }
    
    
    system("pause");
}
/* ============================================================ showMenu
 Δημιουργούμε ένα μενού χρησιμοποιώντας χαρακτήρες asci
*/
void showMenu(int color) {
    std::string dosColors[] = { "7","2" };
    std::string colorPar = "color " + dosColors[color];
    system(colorPar.c_str());    
    system("CLS");
    for (int i = 0;i < 60;i++) std::cout << char(177);std::cout << "\n"; delay(400);
    std::cout << char(177); std::cout << char(177); std::cout << "               KnapsackMania ver 1.0.2                  ";std::cout << char(177); std::cout << char(177);std::cout << '\n';// delay(200);
    std::cout << char(177); std::cout << char(177); std::cout << "    Created by: Ilias Lamprou & Zacharis Aristotelis    ";std::cout << char(177); std::cout << char(177);std::cout << '\n';// delay(200);
    std::cout << char(177); std::cout << char(177); std::cout << "        Copyright (c) 2019 - All rights reserved        ";std::cout << char(177); std::cout << char(177);std::cout << '\n';// delay(200);
    for (int i = 0;i < 60;i++) std::cout << char(177);std::cout << '\n'; delay(500);std::cout << '\n';
    std::cout << char(201);for (int i = 0;i < 58;i++) std::cout << char(205);std::cout << char(187);std::cout << '\n'; 
    std::cout << char(186);std::cout << "                        Main Menu                         ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(204);for (int i = 0;i < 58;i++) std::cout << char(205);std::cout << char(185);std::cout << '\n'; 
    std::cout << char(186);std::cout << " A. CSV files                                             ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(199);for (int i = 0;i < 58;i++) std::cout << char(196);std::cout << char(182);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 0 All algorithms CSV file                                ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 1 Greedy approach CSV file                               ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 2 Brute force CSV file                                   ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 3 Branch and bound CSV file                              ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 4 Dynamic programming CSV file                           ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 5 Dynamic OR-Tools solver CSV file                       ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 6 Integer OR-Tools solver CSV file                       ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(204);for (int i = 0;i < 58;i++) std::cout << char(205);std::cout << char(185);std::cout << '\n'; 
    std::cout << char(186);std::cout << " B. Knapsack Toolbox                                      ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(199);for (int i = 0;i < 58;i++) std::cout << char(196);std::cout << char(182);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 7 Create knapsack problem - Print solution               ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(186);std::cout << " 8 Create all knapsack problems (320 files)               ";std::cout << char(186);std::cout << '\n'; 
    std::string s = std::to_string(knapsack_time_out);
    std::cout << char(186);std::cout << " 9 Configure the timeout (" << s << " milliseconds)";for (int i = 0;i < (18 - s.length());i++)std::cout << char(32); std::cout << char(186);std::cout << '\n';
    std::cout << char(204);for (int i = 0;i < 58;i++) std::cout << char(205);std::cout << char(185);std::cout << '\n'; 
    std::cout << char(186);std::cout << " Other character for Exit                                 ";std::cout << char(186);std::cout << '\n'; 
    std::cout << char(200);for (int i = 0;i < 58;i++) std::cout << char(205);std::cout << char(188);std::cout << '\n'; 
    std::cout << "\n";
    std::cout << "Enter your selection>";
   }


/*============================================================= main
  =============================================================
  Ενφανίζει ένα μενου και περιμένει την επιλογή του χρήστη
  αναλόγως με την επιλογή εκτελείται η αντίστοιχη ρουτίνα
  ή έχουμε έξοδο
 */
int main() {
int selection = -1;
    knapsack_time_out = readTimeOut();
   // system("mode 100");
    int dosColor = 1;
    do {
        showMenu(dosColor);
        dosColor++;
        if (dosColor > 1) dosColor = 0;

        std::cin >> selection;
        std::cin.clear();
     
        if (selection == 0) {
            runAll(0);
            system("pause");
        }
        else if (selection == 1) {
            runAll(1);
            system("pause");
        }
        else if (selection == 2) {
            runAll(2);
            system("pause");
        }
        else if (selection == 3) {
            runAll(3);
            system("pause");
        }
        else if (selection == 4) {
            runAll(4);
            system("pause");
        }
        else if (selection == 5) {
            runAll(5);
            system("pause");
        }
        else if (selection == 6) {
            runAll(6);
            system("pause");
        }
        else if (selection == 7) {
            printOneProblemSolution();
        }
        else if (selection == 8) {
            createAllProblemFiles();
        }
        else if (selection == 9) {
            enterNewTimeOut();
        }
        //else exit(0);
        
       
    } while ((selection>=0) & (selection <=9));

    exit(1);
}