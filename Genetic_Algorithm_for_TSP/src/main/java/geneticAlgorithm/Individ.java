/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package geneticAlgorithm;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Tudor Onofrei
 */
public class Individ implements Comparable<Individ>{
    private float fitness;
    private float probabilitateCross;
    List<Integer> listaNoduri;
    
    public Individ(){
        listaNoduri = new ArrayList<>();
        fitness = 0;
    }
    
    public Individ(List<Integer> permutare){
        fitness = 0;
        listaNoduri = new ArrayList<>();
        listaNoduri.clear();
        listaNoduri.addAll(permutare);
    }
    
    public List<Integer> getListaNoduri(){
        return listaNoduri;
    }
    
    public float getFitness() {
        return fitness;
    }

    public void setFitness(float fitness) {
        this.fitness = fitness;
    }

    public float getProbabilitateCross() {
        return probabilitateCross;
    }

    public void setProbabilitateCross(float probabilitateCross) {
        this.probabilitateCross = probabilitateCross;
    }
    
    @Override
    public String toString(){
        StringBuilder sirIndivid = new StringBuilder();
        
        for(int i = 0; i<listaNoduri.size(); i++){
            sirIndivid.append(listaNoduri.get(i));
            sirIndivid.append(" ");
        }
        
        return sirIndivid.toString();
    }

    @Override
    public int compareTo(Individ altIndivid) {
        if(this.probabilitateCross < altIndivid.probabilitateCross){
            return 1;
        }
        else if(this.probabilitateCross > altIndivid.probabilitateCross){
            return -1;
        }
        return 0;
    }

}
