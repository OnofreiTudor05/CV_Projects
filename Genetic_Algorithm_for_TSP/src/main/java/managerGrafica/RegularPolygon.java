/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package managerGrafica;

import java.awt.Polygon;

/**
 *
 * @author Tudor Onofrei
 */
public class RegularPolygon extends Polygon {
    
    public RegularPolygon(int x0, int y0, int radius, int sides) {
        double alpha = 2 * Math.PI / sides;
        for (int i = 0; i < sides; i++) {
            double x = x0 + radius * Math.cos(alpha * i);
            double y = y0 + radius * Math.sin(alpha * i);
            this.addPoint((int)Math.floor(x), (int)Math.floor(y));
        }
    }
}
