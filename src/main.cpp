#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <vector>

#include <SDL2/SDL.h>
#include <random>
#include <unordered_map>
#include <utility>
#include <thread>
#include <mutex>



//Quit 
bool quit = false;

void keyPress( unsigned char key)
{
    if( key == 'q' )
    {
        quit = true;
    }
}

void close()
{
    SDL_Quit();
}

class Point{
    public:
        int x, y, s_width, s_height, scale;
        char pred_prey;

        Point(int x, int y, char pred_prey, int width, int height, int scale){
            this->x = x;
            this->y = y;
            this->pred_prey = pred_prey;
            this->s_width = width;
            this->s_height = height;
            this->scale = scale;
        }

        void update_position(std::unordered_map<std::string, char>& myMap){
            char value = myMap[std::to_string(x) + "," + std::to_string(y)];
            myMap.erase(std::to_string(x) + "," + std::to_string(y));

            std::pair<int, int> move = checkSurroundings_thenMove(myMap);
            x = move.first;
            y = move.second;
            check_bounds();
            myMap[std::to_string(x) + "," + std::to_string(y)] = value;
        }

    private:
        void check_bounds(){
            if(x < 0) {
                x +=1;
            }
            if(x > s_width / scale - 1){
                x -=1;
            }
            if(y < 0) {
                y +=1;
            }
            if(y > s_height / scale - 1){
                y -=1;
            }
        }

        std::pair<int, int> checkSurroundings_thenMove(std::unordered_map<std::string, char>& myMap){
            int arr[8] = {0};
            std::string address_values[8] = {std::to_string(x-1) + "," + std::to_string(y-1),std::to_string(x-1) + "," + std::to_string(y),std::to_string(x-1) + "," + std::to_string(y+1),std::to_string(x) + "," + std::to_string(y-1),
            std::to_string(x) + "," + std::to_string(y+1),std::to_string(x+1) + "," + std::to_string(y-1),std::to_string(x+1) + "," + std::to_string(y),std::to_string(x+1) + "," + std::to_string(y+1)};
            std::pair<int, int> numeric_address[8] = {std::pair(x-1,y-1), std::pair(x-1,y), std::pair(x-1,y+1),std::pair(x,y-1),std::pair(x,y+1), std::pair(x+1,y-1),std::pair(x+1,y),std::pair(x+1,y+1)};

            for(int i = 0; i < 8; i++){
                arr[i] = myMap.count(address_values[i]);
            }
            

            // If element is a predator
            if(pred_prey == 'X'){
                std::vector<std::pair<int, int>> prey_near_me;
                std::vector<std::string> prey_location;

                for(int i = 0; i < 8; i++){
                    if(arr[i] == 1 && myMap[address_values[i]] == 'O'){
                        prey_near_me.push_back(numeric_address[i]);
                        prey_location.push_back(address_values[i]);
                    }
                }
                
                if(prey_near_me.size() != 0){
                    int randomIndex = rand() % prey_near_me.size();
                    
                    myMap.erase(prey_location[randomIndex]);
                    return prey_near_me[randomIndex];
                } else {
                    int count = 0;
                    bool found = false;
                    while(!found){
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<> dis(0, 7);
                        int guess = dis(gen);

                        if(arr[guess] != 1){
                            return numeric_address[guess];
                            found = true;
                        }
                        if (count > 10){
                            return std::pair(x,y);
                            found = true;
                        }
                        count++;

                    }

                    

                }

            } else {
                int count = 0;
                bool found = false;
                while(!found){
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dis(0, 7);
                    int guess = dis(gen);

                    if(arr[guess] != 1){
                        return numeric_address[guess];
                        found = true;
                    }

                    if (count > 10){
                        return std::pair(x,y);
                        found = true;
                    }
                    count++;
                }
            }
        }   
};

std::mutex mtx;

void render_points(SDL_Renderer* renderer, std::vector<Point>& points, std::unordered_map<std::string, char>& mymap, int& size, int start_val){
    std::lock_guard<std::mutex> lock(mtx);
    for(int i = start_val; i < size; i++){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawPoint(renderer, points[i].x,points[i].y);
        // Only render if the value exists in the map
        if(mymap.count(std::to_string(points[i].x) + "," + std::to_string(points[i].y))){
            points[i].update_position(mymap);
            if(mymap[std::to_string(points[i].x) + "," + std::to_string(points[i].y)] == 'X') {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            }
            

            SDL_RenderDrawPoint(renderer, points[i].x,points[i].y);
        }

    }
    
}


int main(int argc, char *argv[]) {
	
    if (argc != 4) {
		printf("Error. Invalid number of arguments.\n");
		exit(-1);
	}

    const int SCREEN_WIDTH = atoi(argv[1]);
    const int SCREEN_HEIGHT = atoi(argv[2]);
    const int num_points = atoi(argv[3]);
    const int SCALE_FACTOR = 10; //**This is important

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_RenderSetScale(renderer,SCALE_FACTOR,SCALE_FACTOR);

    std::vector<Point> points;
    
    std::unordered_map<std::string, char> mymap;

    for(int i = 0; i < num_points; i++){
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(0, SCREEN_WIDTH / SCALE_FACTOR);
        int x = dis(gen);
        int y = dis(gen);
        char pred_prey;
        
        //Randomly assign elements to be either predator or prey
        if(x % 2 != 0){
            pred_prey = 'X';
        } else {
            pred_prey = 'O';
        }

        mymap[std::to_string(x) + "," + std::to_string(y)] = pred_prey;
        points.push_back(Point(x,y, pred_prey, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE_FACTOR));
        
    }


    SDL_Event e;
    SDL_StartTextInput();
    while(!quit) {

        //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                //Handle keypress with current mouse position
                else if( e.type == SDL_TEXTINPUT )
                {
                    
                    
                    keyPress( e.text.text[ 0 ]);
                }
            }

        int first_half = points.size() / 2;
        int size = points.size();
        int start_val = 0;

        std::thread t1([&](){
            render_points(renderer, points, mymap, first_half, start_val);
        });

        std::thread t2([&](){
            render_points(renderer, points, mymap, size, first_half);
        });
        t1.join();
        t2.join();
        
        SDL_RenderPresent(renderer);
        
        /*
        for(auto const &value: mymap){
            std::cout << "{" << value.first << ": " << value.second << "}\n";
        }
        std::cout << std::endl;
        */
       //std::cout << mymap.size() << std::endl;
        
        SDL_Delay(50); // Dictates the speed of the simulation

    }

    SDL_StopTextInput();

    close();

    return 0;

}