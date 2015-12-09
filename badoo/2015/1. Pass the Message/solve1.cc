/* solve1.cc / Pass the Message Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 */
#include "helper.1.h"

#include <bitset>
#include <queue>

using FriendContacts = std::vector< std::vector<int> >;

int simulate(const FriendContacts& contacts , int friend_id, int N) {

        // vector of notified friends
        std::bitset<100> notified;
        notified.set(friend_id); // friend I call is notified in step 0

        std::queue<int> notify_queue;
        notify_queue.push(friend_id);

        int steps,
            pending;
        for (steps=0,pending = N-1; !notify_queue.empty() && pending ; ++steps) {
            
            int current = notify_queue.front();
            notify_queue.pop();

            for ( const int contact : contacts[current] ) {
                if ( ! notified[contact] ) {
                    -- pending;
                    notified.set(contact);
                    notify_queue.push(contact);
                }
            }
        }

        return pending==0 ? steps : 3*N;
}

int main() {

    // fast input from stdin
    helper::LineReader<> reader;
    std::string line;
	
    // fast output to stdout
	std::string output;
	output.reserve(4096);
    
    // read number of testcases T
    reader.get_line(line);
    const int T = helper::string_to_int(line);
    
    for (int test_count = 0; test_count < T ; ++test_count) {

        // read number of friends N
        reader.get_line(line);
        const int N = helper::string_to_int(line);

        // read friend's contact list into a vector of vectors
        std::vector< std::vector<int> > friends;
        friends.resize(N+1);
        
        // friend[0] is skipped as input is 1-indexed
        for (int i = 1; i <= N ; ++i) {
            reader.get_line(line);
            friends[i] = helper::num_vector<int>(line);
        }
        
        int best_steps = N + N;
        std::vector<int> best_friends;

        // simulate for each friend and keep best choices
        for (int i = 1; i<= N; ++i) {
            const int steps_needed = simulate(friends,i,N);

            if (steps_needed < best_steps) {
                best_steps = steps_needed;
                best_friends.clear();
                best_friends.push_back(i);
            }
            else if (steps_needed == best_steps) {
                best_friends.push_back(i);
            }
        }
        
        int num_friends = best_friends.size();
        for (int i = 0; i < num_friends ; ++i) {
            if (i>0) {
                output.push_back(' ');
            }
            output.append( std::to_string(best_friends[i]) );
        }

        if (num_friends == 0) {
            output.push_back('0');
        }
        output.push_back('\n');
        
    }


	write(1,output.data(),output.size());
}

