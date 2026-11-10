#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TravelRoute{
    public:
    float travel_cost;

    TravelRoute(float cost = 0){
        travel_cost = cost;
    }
};

class StarSystem{
    public:
    int system_id;
    string name;
    int num_planets;
    float x_coord, y_coord, z_coord;
    bool is_habitable;
    vector<StarSystem*> children;
    vector<TravelRoute> routes;     // routes[i] is the cost to reach children[i]

    StarSystem(int id, string n, int planets, float x, float y, float z, bool habitable){
        system_id = id;
        name = n;
        num_planets = planets;
        x_coord = x;
        y_coord = y;
        z_coord = z;
        is_habitable = habitable;
    }
};

class Galaxy{
    public:
    int M;
    StarSystem* root;

    Galaxy(int m){
        M = m;
        root = NULL;
    }

    // the galaxy takes ownership of the root and everything under it
    Galaxy(int m, StarSystem* r){
        M = m;
        root = r;
    }

    ~Galaxy(){
        destroy(root);
    }

    void destroy(StarSystem* x){
        if(x==NULL) return;
        for(int i=0; i<(int)x->children.size(); i++){
            destroy(x->children[i]);
        }
        delete x;
    }

    // links child under parent, fails if parent already has M children
    bool add_route(StarSystem* parent, StarSystem* child, float cost){
        if(parent==NULL || child==NULL) return false;
        if((int)parent->children.size()>=M){
            cout << "Cannot add " << child->name << ": " << parent->name << " already has " << M << " routes" << endl;
            return false;
        }
        parent->children.push_back(child);
        parent->routes.push_back(TravelRoute(cost));
        return true;
    }

    // number of systems in the subtree rooted at x
    int subtree_size(StarSystem* x) const{
        if(x==NULL) return 0;
        int total = 1;
        for(int i=0; i<(int)x->children.size(); i++){
            total += subtree_size(x->children[i]);
        }
        return total;
    }

    // balanced if at every node the child subtree sizes differ by at most 1
    bool is_balanced(StarSystem* x) const{
        if(x==NULL || x->children.empty()) return true;
        int smallest = subtree_size(x->children[0]);
        int largest = smallest;
        for(int i=0; i<(int)x->children.size(); i++){
            int s = subtree_size(x->children[i]);
            if(s<smallest) smallest = s;
            if(s>largest) largest = s;
            if(!is_balanced(x->children[i])) return false;
        }
        return (largest-smallest)<=1;
    }

    int count_leaves(StarSystem* x) const{
        if(x==NULL) return 0;
        if(x->children.empty()) return 1;
        int total = 0;
        for(int i=0; i<(int)x->children.size(); i++){
            total += count_leaves(x->children[i]);
        }
        return total;
    }
};

int main(){
    StarSystem* sol = new StarSystem(1, "Sol", 8, 0, 0, 0, true);
    Galaxy galaxy(3, sol);

    StarSystem* alpha = new StarSystem(2, "Alpha", 4, 1, 2, 3, true);
    StarSystem* beta = new StarSystem(3, "Beta", 2, 4, 1, 0, false);
    StarSystem* gamma = new StarSystem(4, "Gamma", 6, 2, 5, 1, true);
    StarSystem* delta = new StarSystem(5, "Delta", 1, 7, 3, 2, false);
    StarSystem* epsilon = new StarSystem(6, "Epsilon", 3, 5, 5, 5, true);
    StarSystem* zeta = new StarSystem(7, "Zeta", 5, 9, 1, 4, false);
    StarSystem* eta = new StarSystem(8, "Eta", 2, 3, 8, 6, true);

    galaxy.add_route(sol, alpha, 10.5);
    galaxy.add_route(sol, beta, 7.0);
    galaxy.add_route(sol, gamma, 12.25);
    galaxy.add_route(alpha, delta, 3.5);
    galaxy.add_route(alpha, epsilon, 4.0);
    galaxy.add_route(beta, zeta, 6.75);
    galaxy.add_route(gamma, eta, 2.5);

    cout << "Leaf systems: " << galaxy.count_leaves(galaxy.root) << endl;
    cout << "Balanced: " << (galaxy.is_balanced(galaxy.root) ? "Yes" : "No") << endl;

    // extending one branch makes the sizes uneven (4, 2, 2)
    StarSystem* theta = new StarSystem(9, "Theta", 0, 8, 8, 8, false);
    galaxy.add_route(delta, theta, 1.5);

    cout << endl << "After adding Theta under Delta:" << endl;
    cout << "Leaf systems: " << galaxy.count_leaves(galaxy.root) << endl;
    cout << "Balanced: " << (galaxy.is_balanced(galaxy.root) ? "Yes" : "No") << endl;

    // M limit check, Sol already has 3 routes
    StarSystem* extra = new StarSystem(10, "Omega", 1, 0, 0, 9, false);
    if(!galaxy.add_route(sol, extra, 5.0)) delete extra;
}
