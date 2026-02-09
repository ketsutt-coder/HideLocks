#include <Geode/Geode.hpp>
#include <Geode/modify/GJItemIcon.hpp>

using namespace geode::prelude;

// Usamos el macro $modify para engancharnos a la clase GJItemIcon
class $modify(MyHideLocks, GJItemIcon) {
    
    // Método que actualiza el estado del icono (bloqueado/desbloqueado)
    void changeToLockedState(float opacity) {
        // Llamamos a la función original primero para que el juego haga su lógica
        GJItemIcon::changeToLockedState(opacity);

        // Ahora buscamos el sprite del candado y lo ocultamos.
        // En Geometry Dash, el candado suele ser un hijo añadido al icono.
        // Nota: Esto depende de la estructura exacta de nodos, pero una forma segura es:
        
        // Opción A: Forzar que el juego crea que está desbloqueado visualmente
        // (Esto a veces es complejo porque afecta la lógica de selección).

        // Opción B (Más segura visualmente): Buscar el sprite del candado
        if (auto lockSprite = this->getChildByID("lock-sprite")) {
            lockSprite->setVisible(false);
        } else {
            // Si el ID no existe (versiones viejas de Geode/GD), iteramos hijos
            // El candado suele ser un CCSprite encima del icono.
            for (auto* child : CCArrayExt<CCNode*>(this->getChildren())) {
                if (auto* sprite = typeinfo_cast<CCSprite*>(child)) {
                    // Verificamos si la textura parece ser un candado
                    // (Esto requiere saber el nombre del frame, ej: "gj_lock_001.png")
                    if (sprite->getTexture()->getName() /* lógica de chequeo */) {
                        sprite->setVisible(false);
                    }
                }
            }
        }
    }
    
    // Alternativa más simple: Hookear 'create' o 'init' y forzar el desbloqueo
    // Solo haz esto si solo quieres cambio visual
    static GJItemIcon* create(
        UnlockType type, 
        int id, 
        cocos2d::ccColor3B color1, 
        cocos2d::ccColor3B color2, 
        bool p4, 
        bool p5, 
        bool p6, 
        cocos2d::ccColor3B color3
    ) {
        // Llamamos al original
        auto ret = GJItemIcon::create(type, id, color1, color2, p4, p5, p6, color3);
        
        if (ret) {
            // Buscamos el candado inmediatamente después de la creación
            // El candado suele añadirse si el item está bloqueado en el SaveManager.
            
            // Un truco común es ocultar cualquier hijo que parezca un candado.
            // O simplemente forzar la visibilidad del 'lock' a false si tienes el ID.
            
            // Si usas Geode v3.0.0+ con Node IDs:
            if (auto lock = ret->getChildByID("lock-sprite")) {
                lock->setVisible(false);
            }
        }
        return ret;
    }
};
