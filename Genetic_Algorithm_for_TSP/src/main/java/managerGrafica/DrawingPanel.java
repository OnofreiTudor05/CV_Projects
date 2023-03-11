/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package managerGrafica;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.util.List;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import javax.swing.BorderFactory;
import javax.swing.JPanel;

/**
 *
 * @author Tudor Onofrei
 */
public class DrawingPanel extends JPanel {

    final MainFrame frame;
    public static int W, H, oldW, oldH;
    BufferedImage image;
    int xMouse, yMouse, state;
    Image imagineFundal;
    
    List<Point> puncteDesenate = new ArrayList<>();
    public Graphics2D graphics;

    public DrawingPanel(MainFrame frame) {
        this.frame = frame;
        W = 800;
        H = 600;
        oldW = 800;
        oldH = 600;
        this.createOffscreenImage();
        init();
    }

    public void createOffscreenImage() {
        image = new BufferedImage(W, H, BufferedImage.TYPE_INT_ARGB);
        graphics = image.createGraphics();
        graphics.setColor(Color.BLACK);
        graphics.fillRect(0, 0, W, H);
        if(imagineFundal != null){
            frame.canvas.graphics.drawImage(frame.canvas.imagineFundal, 0, 0, W, H, null);
        }
    }

    private void init() {
        setPreferredSize(new Dimension(W, H));
        setBorder(BorderFactory.createEtchedBorder());

        this.addComponentListener(new ComponentAdapter() {

            @Override
            public void componentResized(ComponentEvent e) {
                Component dimensiuni = (Component) e.getSource();
                oldW = W;
                oldH = H;
                W = dimensiuni.getWidth();
                H = dimensiuni.getHeight();   
            }
        });
    }

    public void afiseazaMesaj(String mesaj, int x, int y){
        graphics.setColor(new Color(1f, 1f, 1f, 1f));
        graphics.setFont(new Font("Times New Roman", Font.PLAIN, 50));
        graphics.drawString(mesaj, x, y);
    }
    
    
    void drawCity(CityConfiguration city){
        CityConfiguration rutaNoua = new CityConfiguration(city.getX0(), city.getY0(), (int)city.getRadius(), frame.algoritmGenetic.getInstantaDeRezolvat().getNumarNoduri(), city.getRutaOrase());
        
        frame.canvas.graphics.setColor(Color.YELLOW);
        int index = 0;
        
        for (index = 0; index < city.getRutaOrase().getListaNoduri().size()-1; index++){
            frame.canvas.graphics.drawLine(rutaNoua.xpoints[city.getRutaOrase().getListaNoduri().get(index)], 
                                           rutaNoua.ypoints[city.getRutaOrase().getListaNoduri().get(index)], 
                                           rutaNoua.xpoints[city.getRutaOrase().getListaNoduri().get(index+1)], 
                                           rutaNoua.ypoints[city.getRutaOrase().getListaNoduri().get(index+1)]);
        }
        frame.canvas.graphics.drawLine(rutaNoua.xpoints[city.getRutaOrase().getListaNoduri().get(index)], 
                                           rutaNoua.ypoints[city.getRutaOrase().getListaNoduri().get(index)], 
                                           rutaNoua.xpoints[city.getRutaOrase().getListaNoduri().get(0)], 
                                           rutaNoua.ypoints[city.getRutaOrase().getListaNoduri().get(0)]);
        
        
        frame.canvas.graphics.setColor(Color.WHITE);
        
        for (index = 0; index < rutaNoua.npoints; index++) {
            frame.canvas.graphics.fillOval(rutaNoua.xpoints[index] - 10, rutaNoua.ypoints[index] - 10, 20, 20);
        }
    }
    
    @Override
    public void update(Graphics g) {
    }

    @Override
    protected void paintComponent(Graphics g) {
        g.drawImage(image, 0, 0, this);
    }

}
