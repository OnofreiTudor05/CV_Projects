/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package managerGrafica;

import geneticAlgorithm.Individ;
import java.awt.Polygon;

/**
 *
 * @author Tudor Onofrei
 */
public class CityConfiguration extends Polygon {
    private int radius;
    private Individ rutaOrase;
    private int x0;
    private int y0;
    
    public CityConfiguration(int x0, int y0, int radius, int sides, Individ individInPopulatie) {
        this.x0 = x0;
        this.y0 = y0;
        this.radius = radius;
        double alpha = 2 * Math.PI / sides;
        for (int i = 0; i < sides; i++) {
            double x = x0 + radius * Math.cos(alpha * i);
            double y = y0 + radius * Math.sin(alpha * i);
            this.addPoint((int)Math.floor(x), (int)Math.floor(y));
        }
        rutaOrase =  new Individ(individInPopulatie.getListaNoduri());
    }

    public float getRadius() {
        return radius;
    }

    public void setRadius(int radius) {
        this.radius = radius;
    }

    public Individ getRutaOrase() {
        return rutaOrase;
    }

    public int getX0() {
        return x0;
    }

    public void setX0(int x0) {
        this.x0 = x0;
    }

    public int getY0() {
        return y0;
    }

    public void setY0(int y0) {
        this.y0 = y0;
    }
}
