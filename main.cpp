#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
#include<algorithm>
#include <limits>

using namespace std;

struct Node {
    char song[100];
    Node* next;
    Node* prev;
};

class Playlist {
private:
    Node* head;
    Node* recentlyPlayedTop;

    void writeToFile(const char song[]) {
        ofstream file("playlist.txt", ios::app);
        file << song << endl;
        file.close();
    }

    void deleteFromFile(const char song[]) {
        ifstream infile("playlist.txt");
        ofstream tempfile("temp.txt");
        string line;
        bool found = false;

        while (getline(infile, line)) {
            if (strcmp(song, line.c_str()) != 0)
                tempfile << line << endl;
            else
                found = true;
        }

        infile.close();
        tempfile.close();

        remove("playlist.txt");
        rename("temp.txt", "playlist.txt");

        if (found) cout << "Song deleted from file.\n";
        else cout << "Song not found in file.\n";
    }

    void pushRecentlyPlayed(const char song[]) {
        if (recentlyPlayedTop == nullptr || strcmp(recentlyPlayedTop->song, song) != 0) {
            Node* newNode = new Node();
            strcpy(newNode->song, song);
            newNode->next = recentlyPlayedTop;
            recentlyPlayedTop = newNode;
        }
    }

public:
 Playlist() {
    head = new Node();
    cout << "Enter Playlist name: ";
    cin.getline(head->song, 100); 
    head->next = nullptr;
    head->prev = nullptr;
    recentlyPlayedTop = nullptr;
}

    void addSong() {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }

        Node* newNode = new Node();
        cout << "Enter song name: ";
        cin >> newNode->song;

        current->next = newNode;
        newNode->prev = current;
        newNode->next = nullptr;

        writeToFile(newNode->song);
    }

    void displaySongs() {
        Node* current = head;
        cout << "\nPlaylist: " << head->song << endl;
        current = current->next;
        while (current != nullptr) {
            cout << current->song << endl;
            current = current->next;
        }
    }

    void countSongs() {
        Node* current = head->next;
        int count = 0;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        cout << "\nTotal songs: " << count << endl;
    }

    void searchSong() {
        char query[100];
        cout << "Enter song to search: ";
        cin >> query;

        Node* current = head->next;
        bool found = false;

        while (current != nullptr) {
            if (strstr(strlwr(current->song), strlwr(query))) {
                cout << "Song found: " << current->song << endl;
                found = true;
                break;
            }
            current = current->next;
        }
        if (!found)
            cout << "Song not found.\n";
    }

    void filterSongsByPrefix() {
        char prefix[100];
        cout << "Enter prefix to filter songs: ";
        cin >> prefix;

        Node* current = head->next;
        bool found = false;
        while (current != nullptr) {
            if (strncmp(current->song, prefix, strlen(prefix)) == 0) {
                cout << current->song << endl;
                found = true;
            }
            current = current->next;
        }
        if (!found)
            cout << "No songs with this prefix found.\n";
    }

    void playSong() {
        char song[100];
        cout << "Enter song to play: ";
        cin >> song;

        Node* current = head->next;
        while (current != nullptr) {
            if (strcmp(current->song, song) == 0) {
                cout << "Now playing: " << song << endl;
                pushRecentlyPlayed(song);
                return;
            }
            current = current->next;
        }
        cout << "Song not found.\n";
    }

    void showRecentlyPlayed() {
        Node* current = recentlyPlayedTop;
        if (!current) {
            cout << "No recently played tracks.\n";
            return;
        }

        cout << "Recently played tracks:\n";
        while (current) {
            cout << current->song << endl;
            current = current->next;
        }
    }

    void deleteSongByPosition(int pos) {
        Node* current = head->next;
        int i = 1;

        while (current != nullptr && i < pos) {
            current = current->next;
            i++;
        }

        if (current == nullptr) {
            cout << "Invalid position.\n";
            return;
        }

        deleteFromFile(current->song);
        current->prev->next = current->next;
        if (current->next)
            current->next->prev = current->prev;
        delete current;
        cout << "Deleted song at position " << pos << endl;
    }

    void sortSongs() {
        Node* sorted = nullptr;
        Node* current = head->next;

        while (current != nullptr) {
            Node* next = current->next;
            current->prev = current->next = nullptr;

            if (sorted == nullptr || strcmp(current->song, sorted->song) < 0) {
                current->next = sorted;
                if (sorted) sorted->prev = current;
                sorted = current;
            } else {
                Node* temp = sorted;
                while (temp->next != nullptr && strcmp(temp->next->song, current->song) < 0) {
                    temp = temp->next;
                }
                current->next = temp->next;
                if (temp->next) temp->next->prev = current;
                temp->next = current;
                current->prev = temp;
            }
            current = next;
        }

        head->next = sorted;
    }

    void loadFromFile() {
        ifstream infile("playlist.txt");
        string line;

        while (getline(infile, line)) {
            Node* current = head;
            while (current->next != nullptr) current = current->next;

            Node* newNode = new Node();
            strcpy(newNode->song, line.c_str());
            newNode->next = nullptr;
            newNode->prev = current;
            current->next = newNode;
        }

        infile.close();
        cout << "Playlist loaded from file.\n";
    }
};

int main() {
    Playlist myPlaylist;

    int choice;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Add Song\n2. Display Playlist\n3. Count Songs\n4. Search Song\n5. Filter by Prefix\n";
        cout << "6. Play Song\n7. Recently Played\n8. Delete by Position\n9. Sort Songs\n10. Load from File\n11. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: myPlaylist.addSong(); break;
            case 2: myPlaylist.displaySongs(); break;
            case 3: myPlaylist.countSongs(); break;
            case 4: myPlaylist.searchSong(); break;
            case 5: myPlaylist.filterSongsByPrefix(); break;
            case 6: myPlaylist.playSong(); break;
            case 7: myPlaylist.showRecentlyPlayed(); break;
            case 8: {
                int pos;
                cout << "Enter position to delete: ";
                cin >> pos;
                myPlaylist.deleteSongByPosition(pos);
                break;
            }
            case 9: myPlaylist.sortSongs(); myPlaylist.displaySongs(); break;
            case 10: myPlaylist.loadFromFile(); break;
            case 11: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 11);

    return 0;
}
