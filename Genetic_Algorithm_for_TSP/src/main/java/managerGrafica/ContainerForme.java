/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package managerGrafica;

import static managerGrafica.DrawingPanel.H;
import static managerGrafica.DrawingPanel.W;
import static managerGrafica.DrawingPanel.oldH;
import static managerGrafica.DrawingPanel.oldW;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Tudor Onofrei
 */
public class ContainerForme {

    List<CityConfiguration> listaOrase = new ArrayList<>();
    
    final MainFrame frame;

    public ContainerForme(MainFrame frame) {
        this.frame = frame;
    }

    public void addCity(CityConfiguration orasNou) {
        listaOrase.add(orasNou);
    }

    public void reset() {
        listaOrase.clear();
    }

    public void redraw() {
        frame.canvas.createOffscreenImage();
        
        if (!listaOrase.isEmpty()) {
            for (CityConfiguration oras : listaOrase) {
                frame.canvas.drawCity(oras);
            }
        }
    }

    public void resizeAll() {
        int state = 0;
        
        if (!listaOrase.isEmpty()) {
            for (CityConfiguration poligonInLista : listaOrase) {
                
                poligonInLista.setY0((int)(H * 1.0f/2));
                
                int newRadius1 = (int)(poligonInLista.getRadius() * (W * 1.0f / oldW));
                int newRadius2 = (int)(poligonInLista.getRadius() * (H * 1.0f / oldH));
                
                if(newRadius2 < newRadius1){
                    newRadius1 = newRadius2;
                }
                
                poligonInLista.setRadius(newRadius1);
                
                if(state == 0){
                   poligonInLista.setX0((int)(poligonInLista.getRadius()) + 20);
                }
                else{
                   poligonInLista.setX0((int)(W - poligonInLista.getRadius()) - 20);
                }
                state = 1-state;
                int numarPuncte = poligonInLista.npoints;
                for (int indexPuncte = 0; indexPuncte < numarPuncte; indexPuncte++) {
                    float valoareXNou = (float) poligonInLista.xpoints[indexPuncte] * (W * 1.0f / oldW);
                    float valoareYNou = (float) poligonInLista.ypoints[indexPuncte] * (H * 1.0f / oldH);

                    poligonInLista.xpoints[indexPuncte] = (int) valoareXNou;
                    poligonInLista.ypoints[indexPuncte] = (int) valoareYNou;
                }
            }
        }
    }
}
