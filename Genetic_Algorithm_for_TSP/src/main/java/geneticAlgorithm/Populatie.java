/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package geneticAlgorithm;

/**
 *
 * @author Tudor Onofrei
 */
import geneticAlgorithm.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Populatie {

    private List<Individ> listaIndivizi;

    public Populatie(){
        listaIndivizi =  new ArrayList<>();
    }
    
    public Populatie(int numarIndivizi, int dimensiuneIndivid) {
        listaIndivizi = new ArrayList<>();
        genereazaPopulatie(numarIndivizi, dimensiuneIndivid);
    }
   
    public void genereazaPopulatie(int numarIndivizi, int dimensiuneIndivid) {
        for (int ii = 0; ii < numarIndivizi; ii++) {
            List<Integer> permutareCreata = new ArrayList<>();
            permutareCreata.clear();
            for (int i = 0; i < dimensiuneIndivid; i++) {
                permutareCreata.add(i);
            }
            Collections.shuffle(permutareCreata);
            listaIndivizi.add(new Individ(permutareCreata));
        }
    }
    
    public List<Individ> getListaIndivizi(){
        return listaIndivizi;
    }
    
    @Override
    public String toString(){
        StringBuilder StringInstantaPopulatie = new StringBuilder();
        
        StringInstantaPopulatie.append("Numar indivizi: ");
        StringInstantaPopulatie.append(listaIndivizi.size());
        StringInstantaPopulatie.append("\n");
        for(Individ i : listaIndivizi){
            for(Integer j : i.getListaNoduri()){
                StringInstantaPopulatie.append(j);
                StringInstantaPopulatie.append(" ");
            }
            StringInstantaPopulatie.append("\n");
        }
        
        
        return StringInstantaPopulatie.toString();
    }
}
