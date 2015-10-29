#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

// STD
#include <map>
#include <string>
#include <utility>
#include <type_traits>

#include "Camera.h"

#include "Singleton.h"

class CameraManager : public Singleton<CameraManager>
{
private:
    Camera* m_pActiveCamera;                        // The Camera currently in-use 
    Camera* m_pDebugCamera;                         // An instance of the DebugCamera 
    std::map<std::string, Camera*> m_cameras;       // std:::map of all registered Cameras

	bool debugActive;
public:
    CameraManager(void);
    virtual ~CameraManager(void);

    /*
     * Returns an instance of the in-use Camera.
     */
    Camera* const GetActiveCamera( void ) const;

	/*
	 * Returns whether the debug camera is the active camera
	 */
	bool IsDebugActive() { return debugActive; }

    /*
     * Attempt to retrieve the Camera associated with the provided id.
     * @param   id      The id of the Camera to locate.
     */
    Camera* const GetCameraByID( const std::string& id ) const;

    /*
     * Set the in-use Camera.
     * @param   id      The id associated with that Camera
     * @return  Boolean indicating if this operation was successful.
     */
    bool SetActiveCamera( const std::string& id );

    /*
     * Sets the DebugCamera as the in-use Camera.
     */
    void ActivateDebugCamera( void );

    /*
     * Update the CameraManager.
     * @param   deltaTime   The delta time value for this frame.
     */
    void Update( float deltaTime );

    /*
     * Register a Camera with the CameraManager.
     * @param   id      The id to associate with this Camera.
     * @param   args    The arguments passed to the constructor for T.
     * @return  A boolean indicating if this operation was successful.
     *
     * The following example registers a DebugCamera situated at the origin:
     *      CameraManager* pManager = CameraManager::instance();
     *      pManager->RegisterCamera<DebugCamera>( "debug_camera", 0.0f, 0.0f, 0.0f );
     *      pManager->SetActiveCamera( "debug_camera" );
     */
    template <typename T, typename... Args>
    bool RegisterCamera( const std::string& id, Args&&... args )
    {
        // Assert that T is a child of the Camera class.
        static_assert( std::is_base_of<Camera, T>::value, "Must be an instance of class Camera!" );

        // See if there is already a Camera* stored at that id.
        auto& iter = m_cameras.find( id );
        if( iter != m_cameras.cend() )
        {
            return false;
        }

        // Construct the Camera.
        Camera* c = new T( std::forward<Args>( args )... );
        m_cameras.emplace( id, c );

        return true;
    }
};

#endif
