/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package jtrickynoid;

import java.util.Iterator;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import static jtrickynoid.BallSystem.ballGfx;
import static jtrickynoid.JTrickyNoid.echo;
import org.newdawn.slick.Animation;
import org.newdawn.slick.Color;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.SpriteSheet;
import org.newdawn.slick.tiled.TiledMap;

/**
 *
 * @author Nishi
 */
public class BrickSystem {
    private Brick[] bricks;    
    private int bricksLeft;
    
    /* EXTERNAL CLASS REFERENCES */
    private Game game;
            
    public BrickSystem(Game game) {
        bricks = null;
        this.game = game;
    }
    
    private static Animation[] brickGfx;
    
    public void update(int Delta){
        for(Brick b: bricks){
            
            // Check fall
            if(b.isFalling()){
                //b.fall();
            }
            
            
            
        }
    }
    
    public static void loadGraphics(){
           try {
            brickGfx = new Animation[0xA0];
            
            SpriteSheet brickSheet = new SpriteSheet("data/gfx/bricks.bmp", 16, 8, new Color(255,0,255));
            int i=0;
            for(int y=0; y<10; y++){
                for(int x=0; x<16; x++){                
                    brickGfx[i] = new Animation(brickSheet, x, y, x, y, false, 1, false);        
                    i++;
                }
            }
        } catch (SlickException ex) {
            brickGfx = null;
            echo("Cannot load bricks.bmp!");
        }
    }
    
    public void loadBricks(TiledMap mapa){
        int x;
        int y;
        int brickIndex = 0;
    
        if(this.bricks == null) {
            // TODO: Parametrize brick array size
            this.bricks = new Brick[26 * 14];
        }
        
        for(y=0; y<26; y++){
            for(x=0; x<14; x++){
                bricks[brickIndex]= new Brick(x, y, mapa.getTileId(x, y, 0), mapa.getTileId(x, y, 2) -  0x81);
                brickIndex++;                
            }
        }
    }
    
    public boolean test(int x, int y) {
        //x/=16;
       // y/=8;
        for(Brick b : bricks){
            if(b.test(x, y)){
                return b.isAlive();
            }
        }
        return false;
    }
    
    public boolean hit(int x, int y) {
        int brickX = x/16;
        int brickY = y/8;
        int brickId = (brickY * 14) + brickX;
        
        boolean causesBounce = false;
        
        if(brickX > 14) return true;
        if(brickX < 0 ) return true;
        if(brickY > 25) return true;
        if(brickY < 0 ) return true;
        
        Brick b = bricks[brickId];
        
        if(b.isAlive()){
            
            
            causesBounce = b.causesBounce();
            if(!b.isSticked()){
                b.fall();
            }
            
            if(b.isInverter()){
                game.reaction.invertDeltas = true;//swapDeltas(game.reaction.target);                
            }
            
            if(b.isDestructable()) {            
                int damage = game.reaction.target.getDamage();
                if(damage == 9)causesBounce = false;
                b.applyDamage(damage);
                echo("Generate particle: "+b.getGraph());
                game.getParticles().generate(b.getGraph(), x+2, y+2);
                game.getBackdrop().setColor(b.getGraph()-1);
            }

            if(b.getDamage() >= 9) {                
                
                game.addScore(b.getScore());
                
                if(b.getTokenType() > 0x00) {
                    game.addToken(b.generateToken());
                }
                b.kill();                
            }
        }
        
        return causesBounce;
    }
    
    
    public void render(Graphics g) {
        int x;
        int y;
        int i;
        int bx;
        int by;
        Brick b;
        
        i = 0;
        for(y = 0 ; y < 26 ; y++){
            for(x = 0 ; x < 14 ; x++){
                b = bricks[i];
                if(b.isAlive()) {
                    
                    // Calculate brick screen absolute position
                    bx = 3 + ( b.getX()*16 );
                    by = 3 + ( b.getY()*8 ) + b.getOffset();
                    
                    // Draw Damage        
                    g.setDrawMode(Graphics.MODE_ALPHA_MAP);
                    brickGfx[0x90+(b.getDamage()*(4/b.getHealth()))].draw(bx, by);
                    
                    // Draw Brick
                    if(b.getGraph() < 0x9C){
                        // Draw Shadow
                        g.setDrawMode(Graphics.MODE_ALPHA_BLEND);
                        brickGfx[0x90].draw(bx+2, by+2);
                    
                        // Draw brick (with damage and shadow)
                        g.setDrawMode(Graphics.MODE_ALPHA_BLEND);                    
                        brickGfx[b.getGraph()-1].draw(bx, by);
                    } else {
                        // Draw 
                        g.setDrawMode(Graphics.MODE_NORMAL);
                        brickGfx[b.getGraph()-1].draw(bx, by);
                    }       
                    
                }
                
                i++;
            }
        }
        
        g.setDrawMode(Graphics.MODE_NORMAL);
          
    }
    
    /* ACCESOR METHODS */
    public Brick[] getBricks() {
        return bricks;
    }
    
    public Brick get(int index){
        return bricks[index];
    }
    
}
