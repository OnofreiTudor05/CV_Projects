/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package geneticAlgorithm;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import javax.swing.Timer;
import managerGrafica.CityConfiguration;
import static managerGrafica.DrawingPanel.H;
import static managerGrafica.DrawingPanel.W;
import managerGrafica.MainFrame;

/**
 *
 * @author Tudor Onofrei
 */
public class GeneticAlgoritm {

    private int numarGeneratii;
    private int dimensiunePopulatie;
    private float probabilitateCrossOver;
    private float probabilitateMutatie;
    private Populatie populatieNoua;
    private Populatie populatieVeche;
    private ProblemInstance instantaDeRezolvat;
    private Random uniformGenerator;
    private Timer timer;
    private int generatieCurenta = 1;
    private Individ bestFromAll;
    public int distantaMinima = 300000;
    
    public GeneticAlgoritm(ProblemInstance instantaProblema) {
        uniformGenerator = new Random();
        numarGeneratii = 2000;
        dimensiunePopulatie = 200;
        probabilitateCrossOver = 0.4f;
        probabilitateMutatie = 0.70f;

        instantaDeRezolvat = instantaProblema;
    }

    public void solve(ProblemInstance instantaProblema, MainFrame frame) {
        populatieVeche = new Populatie(dimensiunePopulatie, instantaProblema.getDimensiune());
        populatieNoua = new Populatie();
        
        
        timer = new Timer(500, new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                frame.algoritmGenetic.setProbabilitateCrossOver(Float.parseFloat(frame.optionPanel.probabilitateCrossSpinner.getValue().toString()));
                frame.algoritmGenetic.setProbabilitateMutatie(Float.parseFloat(frame.optionPanel.probabilitateMutatieSpinner.getValue().toString()));
        
                
                float cost;
                for (Individ j : populatieVeche.getListaIndivizi()) {
                    cost = 0;
                    for (int jj = 0; jj < j.getListaNoduri().size() - 1; jj++) {
                        cost += instantaDeRezolvat.getCost(j.getListaNoduri().get(jj), j.getListaNoduri().get(jj + 1));
                    }
                    cost += instantaDeRezolvat.getCost(j.getListaNoduri().get(j.getListaNoduri().size() - 1), j.getListaNoduri().get(0));

                    if (cost < distantaMinima && cost > 0) {
                        distantaMinima = (int)cost;
                        bestFromAll = new Individ(j.getListaNoduri());
                    }
                }

               
                selecteazaPopulatieTurneu();
                aplicaCrossOver_I();
                aplicaMutatie();
                evolutionChange();

                Populatie populatieOffspring = new Populatie();
                for (Individ ii : populatieNoua.getListaIndivizi()) {
                    populatieOffspring.getListaIndivizi().add(new Individ(ii.getListaNoduri()));
                }
                populatieVeche = populatieOffspring;

                frame.containerForms.reset();

                Individ randomIndivid = new Individ(frame.algoritmGenetic.getRandomIndivid().getListaNoduri());
                frame.containerForms.addCity(new CityConfiguration(200, 300, 150, frame.algoritmGenetic.getInstantaDeRezolvat().getNumarNoduri(), new Individ(randomIndivid.getListaNoduri())));
                frame.containerForms.addCity(new CityConfiguration(600, 300, 150, frame.algoritmGenetic.getInstantaDeRezolvat().getNumarNoduri(), new Individ(frame.algoritmGenetic.getBestFromAll().getListaNoduri())));

                frame.containerForms.resizeAll();
                frame.containerForms.redraw();
                frame.canvas.repaint();

                StringBuilder mesajDeAfisat = new StringBuilder();
                mesajDeAfisat.append("Generatia: #");
                mesajDeAfisat.append(generatieCurenta);
                frame.canvas.afiseazaMesaj(mesajDeAfisat.toString(), 400, 40);

                mesajDeAfisat.setLength(0);
                mesajDeAfisat.append("Evaluat: ");

                cost = 0;
                for (int jj = 0; jj < randomIndivid.getListaNoduri().size() - 1; jj++) {
                    cost += instantaDeRezolvat.getCost(randomIndivid.getListaNoduri().get(jj), randomIndivid.getListaNoduri().get(jj + 1));
                }
                cost += instantaDeRezolvat.getCost(randomIndivid.getListaNoduri().get(randomIndivid.getListaNoduri().size() - 1), randomIndivid.getListaNoduri().get(0));

                mesajDeAfisat.append(cost);
                frame.canvas.afiseazaMesaj(mesajDeAfisat.toString(), 20, (int)(H - mesajDeAfisat.toString().length()));

                
                mesajDeAfisat.setLength(0);
                mesajDeAfisat.append("Minim: ");

                mesajDeAfisat.append(distantaMinima);
                frame.canvas.afiseazaMesaj(mesajDeAfisat.toString(), (int)((W - mesajDeAfisat.toString().length() * 50)), (int)(H -mesajDeAfisat.toString().length()));
 
                
                generatieCurenta++;
                if (generatieCurenta > numarGeneratii) {
                    timer.stop();
                }
            }
        }
        );
        timer.setRepeats(true);
        timer.setCoalesce(true);
        timer.setInitialDelay(0);
        timer.start();

    }

    public void evolutionChange() {
        for (Individ i : populatieNoua.getListaIndivizi()) {
            int pos1 = (int) (uniformGenerator.nextFloat() * i.getListaNoduri().size());
            int pos2 = (int) (uniformGenerator.nextFloat() * i.getListaNoduri().size());

            int aux = i.getListaNoduri().get(pos1);
            i.getListaNoduri().set(pos1, i.getListaNoduri().get(pos2));
            i.getListaNoduri().set(pos2, aux);
        }
    }

    public void selecteazaPopulatieTurneu() {
        populatieNoua.getListaIndivizi().clear();
        populatieNoua = new Populatie();
        for (int i = 0; i < dimensiunePopulatie; i++) {
            int poz1 = uniformGenerator.nextInt(dimensiunePopulatie);
            int poz2 = uniformGenerator.nextInt(dimensiunePopulatie);
            if (getFitness(populatieVeche.getListaIndivizi().get(poz2)) < getFitness(populatieVeche.getListaIndivizi().get(poz1))) {
                Individ individNou = new Individ(populatieVeche.getListaIndivizi().get(poz1).getListaNoduri());
                populatieNoua.getListaIndivizi().add(individNou);
            } else {
                Individ individNou = new Individ(populatieVeche.getListaIndivizi().get(poz2).getListaNoduri());
                populatieNoua.getListaIndivizi().add(individNou);
            }
        }
    }

    public Individ getRandomIndivid() {
        return new Individ(populatieVeche.getListaIndivizi().get(0).getListaNoduri());

    }

    public void aplicaCrossOver_I() {
        Populatie populatieOffspring = new Populatie();

        for (int indiceIndivid = 0; indiceIndivid < populatieNoua.getListaIndivizi().size(); indiceIndivid++) {
            populatieNoua.getListaIndivizi().get(indiceIndivid).setProbabilitateCross((float) uniformGenerator.nextFloat());
        }
        Collections.sort(populatieNoua.getListaIndivizi());
        int indiceIncrucisare = 0;
        while (populatieNoua.getListaIndivizi().get(indiceIncrucisare).getProbabilitateCross() < probabilitateCrossOver) {
            indiceIncrucisare++;
            if (indiceIncrucisare == 100) {
                return;
            }
        }

        if ((indiceIncrucisare + 1) % 2 == 1) {
            indiceIncrucisare--;
        }

        int punctCross = (int) uniformGenerator.nextFloat() * (populatieNoua.getListaIndivizi().get(0).getListaNoduri().size() - 1);

        List<Integer> copil1 = new ArrayList<>();
        List<Integer> copil2 = new ArrayList<>();

        for (int indiceIndivid = 0; indiceIndivid < indiceIncrucisare; indiceIndivid += 2) {
            copil1.clear();
            copil2.clear();
            for (int copiere_noduri = 0; copiere_noduri < punctCross; copiere_noduri++) {
                copil1.add(populatieNoua.getListaIndivizi().get(indiceIndivid).getListaNoduri().get(copiere_noduri));
                copil2.add(populatieNoua.getListaIndivizi().get(indiceIndivid + 1).getListaNoduri().get(copiere_noduri));
            }
            int numarNoduri = instantaDeRezolvat.getNumarNoduri();
            int aparitie_pos = 0;
            for (int copiere_noduri = punctCross; copiere_noduri < numarNoduri; copiere_noduri++) {
                boolean gasit;
                gasit = false;
                for (int aux = 0; aux < numarNoduri && !gasit; aux++) {
                    if (!copil1.contains(populatieNoua.getListaIndivizi().get(indiceIndivid + 1).getListaNoduri().get(copiere_noduri))) {
                        aparitie_pos = aux;
                        gasit = true;
                    }
                }
                copil1.add(populatieNoua.getListaIndivizi().get(indiceIndivid + 1).getListaNoduri().get(aparitie_pos));
            }

            for (int copiere_noduri = punctCross; copiere_noduri < numarNoduri; copiere_noduri++) {
                boolean gasit;
                gasit = false;
                for (int aux = 0; aux < numarNoduri && !gasit; aux++) {
                    if (!copil2.contains(populatieNoua.getListaIndivizi().get(indiceIndivid).getListaNoduri().get(copiere_noduri))) {
                        aparitie_pos = aux;
                        gasit = true;
                    }
                }
                copil2.add(populatieNoua.getListaIndivizi().get(indiceIndivid).getListaNoduri().get(aparitie_pos));
            }

            populatieNoua.getListaIndivizi().remove(populatieNoua.getListaIndivizi().get(indiceIndivid));
            populatieNoua.getListaIndivizi().remove(populatieNoua.getListaIndivizi().get(indiceIndivid));
            populatieNoua.getListaIndivizi().add(0, new Individ(copil1));
            populatieNoua.getListaIndivizi().add(0, new Individ(copil2));
        }
        for (Individ i : populatieNoua.getListaIndivizi()) {
            populatieOffspring.getListaIndivizi().add(new Individ(i.getListaNoduri()));
        }
        populatieNoua = populatieOffspring;
    }

    public void aplicaCrossOver() {
        Populatie populatieOffspring = new Populatie();

        float[] probabilitateIncrucisare = new float[populatieNoua.getListaIndivizi().size() + 1];
        int[] indiciIndivizi = new int[populatieNoua.getListaIndivizi().size() + 1];

        for (int indiceIndivid = 0; indiceIndivid < populatieNoua.getListaIndivizi().size(); indiceIndivid++) {
            probabilitateIncrucisare[indiceIndivid] = (float) uniformGenerator.nextFloat();
            indiciIndivizi[indiceIndivid] = indiceIndivid;
        }

        for (int i = 0; i < populatieNoua.getListaIndivizi().size() - 1; i++) {
            for (int j = i + 1; j < populatieNoua.getListaIndivizi().size(); j++) {
                if (probabilitateIncrucisare[j] < probabilitateIncrucisare[i]) {
                    float auxiliar1 = probabilitateIncrucisare[i];
                    probabilitateIncrucisare[i] = probabilitateIncrucisare[j];
                    probabilitateIncrucisare[j] = auxiliar1;

                    int auxiliar = indiciIndivizi[i];
                    indiciIndivizi[i] = indiciIndivizi[j];
                    indiciIndivizi[j] = auxiliar;
                }
            }
        }

        int indiceStartIncrucisare = 0;
        while (probabilitateIncrucisare[indiceStartIncrucisare] < probabilitateCrossOver) {
            indiceStartIncrucisare++;
        }

        if ((indiceStartIncrucisare + 1) % 2 == 1) {
            indiceStartIncrucisare--;
        }

        int aparitiiNoduriIndivid1[] = new int[populatieNoua.getListaIndivizi().get(0).getListaNoduri().size() + 1];
        int aparitiiNoduriIndivid2[] = new int[populatieNoua.getListaIndivizi().get(0).getListaNoduri().size() + 1];

        for (int i = 0; i <= indiceStartIncrucisare; i += 2) {
            for (int j = 0; j < populatieNoua.getListaIndivizi().get(0).getListaNoduri().size(); j++) {
                aparitiiNoduriIndivid1[j] = aparitiiNoduriIndivid2[j] = 0;
            }

            int punctCross = (int) uniformGenerator.nextFloat() * (populatieNoua.getListaIndivizi().get(0).getListaNoduri().size() - 1);

            int copil1[] = new int[populatieNoua.getListaIndivizi().get(0).getListaNoduri().size() + 1];
            int copil2[] = new int[populatieNoua.getListaIndivizi().get(0).getListaNoduri().size() + 1];

            for (int copiere_noduri = 0; copiere_noduri < punctCross; copiere_noduri++) {
                copil1[copiere_noduri] = populatieNoua.getListaIndivizi().get(indiciIndivizi[i]).getListaNoduri().get(copiere_noduri);
                aparitiiNoduriIndivid1[populatieNoua.getListaIndivizi().get(indiciIndivizi[i]).getListaNoduri().get(copiere_noduri)] = 1;

                copil2[copiere_noduri] = populatieNoua.getListaIndivizi().get(indiciIndivizi[i + 1]).getListaNoduri().get(copiere_noduri);
                aparitiiNoduriIndivid2[populatieNoua.getListaIndivizi().get(indiciIndivizi[i + 1]).getListaNoduri().get(copiere_noduri)] = 1;
            }

            int aparitie_pos = 0;
            for (int poz = punctCross; poz < populatieNoua.getListaIndivizi().get(0).getListaNoduri().size(); poz++) {
                for (int aux = 0; aux < populatieNoua.getListaIndivizi().get(0).getListaNoduri().size(); aux++) {
                    if (aparitiiNoduriIndivid1[copil1[aux]] == 0) {
                        aparitie_pos = aux;
                        aparitiiNoduriIndivid1[copil1[aux]] = 1;
                        break;
                    }
                }
                copil1[poz] = populatieNoua.getListaIndivizi().get(indiciIndivizi[i + 1]).getListaNoduri().get(aparitie_pos);
            }

            for (int poz = punctCross; poz < populatieNoua.getListaIndivizi().get(0).getListaNoduri().size(); poz++) {
                for (int aux = 0; aux < populatieNoua.getListaIndivizi().get(0).getListaNoduri().size(); aux++) {
                    if (aparitiiNoduriIndivid2[copil2[aux]] == 0) {
                        aparitie_pos = aux;
                        aparitiiNoduriIndivid2[copil2[aux]] = 1;
                        break;
                    }
                }
                copil2[poz] = populatieNoua.getListaIndivizi().get(indiciIndivizi[i]).getListaNoduri().get(aparitie_pos);
            }

            for (int copiere = 0; copiere < populatieNoua.getListaIndivizi().get(0).getListaNoduri().size(); copiere++) {
                populatieNoua.getListaIndivizi().get(indiciIndivizi[i]).getListaNoduri().set(copiere, copil1[copiere]);
                populatieNoua.getListaIndivizi().get(indiciIndivizi[i + 1]).getListaNoduri().set(copiere, copil2[copiere]);
            }
        }
        for (Individ i : populatieNoua.getListaIndivizi()) {
            populatieOffspring.getListaIndivizi().add(new Individ(i.getListaNoduri()));
        }
        populatieNoua = populatieOffspring;

    }

    public float getFitness(Individ individDeEvaluat) {
        float cost = 0;
        for (int i = 0; i < individDeEvaluat.getListaNoduri().size() - 1; i++) {
            cost += instantaDeRezolvat.getCost(individDeEvaluat.getListaNoduri().get(i), individDeEvaluat.getListaNoduri().get(i + 1));
        }
        cost += instantaDeRezolvat.getCost(individDeEvaluat.getListaNoduri().get(individDeEvaluat.getListaNoduri().size() - 1), individDeEvaluat.getListaNoduri().get(0));

        return (float) (1.0f / (cost + 1f));
    }

    public void aplicaMutatie_I() {
        Populatie populatieOffspring = new Populatie();

        for (Individ i : populatieNoua.getListaIndivizi()) {
            float probabilitate = (float) uniformGenerator.nextFloat();
            if (probabilitate < probabilitateMutatie) {
                //       System.out.println("\n_!------------------------------Aplicam Mutatia!");
                int pos1 = (int) uniformGenerator.nextFloat() * (i.getListaNoduri().size() - 1);
                int pos2 = (int) uniformGenerator.nextFloat() * (i.getListaNoduri().size() - 1);

                int auxSwap = i.getListaNoduri().get(pos1);
                i.getListaNoduri().set(pos1, i.getListaNoduri().get(pos2));
                i.getListaNoduri().set(pos2, auxSwap);
            }
            populatieOffspring.getListaIndivizi().add(new Individ(i.getListaNoduri()));
        }
        populatieNoua = populatieOffspring;

    }

    public void aplicaMutatie() {
        Populatie populatieOffspring = new Populatie();

        for (Individ i : populatieNoua.getListaIndivizi()) {
            float probabilitate = (float) uniformGenerator.nextFloat();
            if (probabilitate < probabilitateMutatie) {
                Individ individ1 = new Individ(i.getListaNoduri());
                Individ individ2 = new Individ(i.getListaNoduri());
                Individ individ3 = new Individ(i.getListaNoduri());

                int pos1 = (int) uniformGenerator.nextFloat() * (i.getListaNoduri().size() - 1);
                int pos2 = (int) uniformGenerator.nextFloat() * (i.getListaNoduri().size() - 1);

                if (pos1 > pos2) {
                    int auxiliar = pos1;
                    pos1 = pos2;
                    pos2 = auxiliar;
                }

                for (int indiceInverse = pos1; indiceInverse <= pos2; indiceInverse++) {
                    individ1.getListaNoduri().set(indiceInverse, individ1.getListaNoduri().get(individ1.getListaNoduri().size() - 1 - indiceInverse));
                }

                pos1 = (int) uniformGenerator.nextFloat() * (i.getListaNoduri().size() - 1);
                pos2 = (int) uniformGenerator.nextFloat() * (i.getListaNoduri().size() - 1);

                if (pos1 < pos2) {
                    int auxInit = individ2.getListaNoduri().get(pos2);
                    for (int indiceInsert = pos1 + 1; indiceInsert < pos2; indiceInsert++) {
                        individ2.getListaNoduri().set(indiceInsert, individ2.getListaNoduri().get(indiceInsert + 1));
                    }
                    individ2.getListaNoduri().set(pos1, auxInit);
                } else if (pos1 > pos2) {
                    int auxInit = individ2.getListaNoduri().get(pos2);
                    for (int indiceInsert = pos2; indiceInsert < pos1; indiceInsert++) {
                        individ2.getListaNoduri().set(indiceInsert, individ2.getListaNoduri().get(indiceInsert + 1));
                    }
                    individ2.getListaNoduri().set(pos1, auxInit);
                }

                pos1 = (int) uniformGenerator.nextFloat() * (i.getListaNoduri().size() - 1);
                pos2 = (int) uniformGenerator.nextFloat() * (i.getListaNoduri().size() - 1);

                int auxSwap = individ3.getListaNoduri().get(pos1);
                individ3.getListaNoduri().set(pos1, individ3.getListaNoduri().get(pos2));
                individ3.getListaNoduri().set(pos2, auxSwap);

                float Fitness1 = getFitness(individ1);
                float Fitness2 = getFitness(individ2);
                float Fitness3 = getFitness(individ3);
                float maxim = Fitness1;
                for (int jj = 0; jj < i.getListaNoduri().size(); jj++) {
                    i.getListaNoduri().set(jj, individ1.getListaNoduri().get(jj));
                }

                if (maxim < Fitness2) {
                    for (int jj = 0; jj < i.getListaNoduri().size(); jj++) {
                        i.getListaNoduri().set(jj, individ2.getListaNoduri().get(jj));
                    }
                    maxim = Fitness2;
                }

                if (maxim < Fitness3) {
                    for (int jj = 0; jj < i.getListaNoduri().size(); jj++) {
                        i.getListaNoduri().set(jj, individ3.getListaNoduri().get(jj));
                    }
                    maxim = Fitness3;
                }
            }
        }
        for (Individ i : populatieNoua.getListaIndivizi()) {
            populatieOffspring.getListaIndivizi().add(new Individ(i.getListaNoduri()));
        }
        populatieNoua = populatieOffspring;
    }

    public void selfSolve(MainFrame frame) {
        this.solve(this.instantaDeRezolvat, frame);
    }

    public int getNumarGeneratii() {
        return numarGeneratii;
    }

    public void setNumarGeneratii(int numarGeneratii) {
        this.numarGeneratii = numarGeneratii;
    }

    public int getDimensiunePopulatie() {
        return dimensiunePopulatie;
    }

    public void setDimensiunePopulatie(int dimensiunePopulatie) {
        this.dimensiunePopulatie = dimensiunePopulatie;
    }

    public float getProbabilitateCrossOver() {
        return probabilitateCrossOver;
    }

    public void setProbabilitateCrossOver(float probabilitateCrossOver) {
        this.probabilitateCrossOver = probabilitateCrossOver;
    }

    public float getProbabilitateMutatie() {
        return probabilitateMutatie;
    }

    public void setProbabilitateMutatie(float probabilitateMutatie) {
        this.probabilitateMutatie = probabilitateMutatie;
    }

    public ProblemInstance getInstantaDeRezolvat() {
        return instantaDeRezolvat;
    }

    public Individ getBestFromAll() {
        return bestFromAll;
    }
}
