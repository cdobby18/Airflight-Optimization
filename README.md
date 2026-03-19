# Airflight Optimization ✈️

Airflight Optimization is a **C++ console-based application** that simulates flight scheduling, crew assignment, and aircraft management. It uses algorithmic techniques to optimize flight operations, manage crew availability, and handle aircraft maintenance.

## Project Overview

This project models a small network of airports and flights, providing:

- Flight cost calculation using **Dijkstra’s algorithm**  
- Dynamic **flight scheduling** with departures and arrivals  
- Aircraft management with **maintenance tracking** and flight hours  
- Crew assignment including pilots, copilots, and flight attendants  
- Simulation of daily operations, including departures, arrivals, and rest periods  

## Features

- **Flight Cost Optimization:** Calculates shortest paths and prioritizes high-demand flights  
- **Aircraft Management:** Tracks plane hours, schedules maintenance, and assigns planes to flights  
- **Crew Scheduling:** Randomly generates crew availability and assigns crew based on aircraft type  
- **Simulation Mode:** Displays hourly departures and arrivals, crew rest, and aircraft maintenance  
- **Interactive Console UI:** Users can choose airports, view schedules, and run simulations  
  
## How It Works

1. Initialize aircraft from a text file.  
2. Assign crews to aircraft types (e.g., 747 or 330).  
3. Users select an airport to view or process flights.  
4. High-demand flights are prioritized and scheduled using optimized algorithms.  
5. Simulation mode displays hourly flight operations, including departures, arrivals, and maintenance events.  

## Use Cases

- Demonstrates practical use of **graph algorithms** for shortest path calculation  
- Models **real-world flight and crew scheduling** problems  
- Showcases **object-oriented programming**, data structures, and algorithmic problem solving in C++  
