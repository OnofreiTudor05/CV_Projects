using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.ARFoundation;
using UnityEngine.XR.ARSubsystems;
using UnityEngine.UI;

namespace UnityEngine.XR.ARFoundation.Samples
{
    /// <summary>
    /// Listens for touch events and performs an AR raycast from the screen touch point.
    /// AR raycasts will only hit detected trackables like feature points and planes.
    ///
    /// If a raycast hits a trackable, the <see cref="gameScene"/> is instantiated
    /// and moved to the hit position.
    /// </summary>
    [RequireComponent(typeof(ARRaycastManager))]
    public class PlaceOnPlane : MonoBehaviour
    {
        [SerializeField]
        [Tooltip("Instantiates this prefab on a plane at the touch location.")]
        GameObject sceneReference;

        [SerializeField]
        Slider slider;

        // The position without offset
        [SerializeField]
        private float raycastHeight = 0f;

        /// <summary>
        /// The game scene
        /// </summary>
        public GameObject gameScene
        {
            get { return sceneReference; }
            set { sceneReference = value; }
        }

        void Awake()
        {
            raycastHeight = gameScene.transform.position.y;
            m_RaycastManager = GetComponent<ARRaycastManager>();
        }

        bool TryGetTouchPosition(out Vector2 touchPosition)
        {
            if (Input.touchCount > 0)
            {
                touchPosition = Input.GetTouch(0).position;
                return true;
            }

            touchPosition = default;
            return false;
        }

        void Update()
        {
            if (!TryGetTouchPosition(out Vector2 touchPosition))
                return;

            if (m_RaycastManager.Raycast(touchPosition, s_Hits, TrackableType.PlaneWithinPolygon))
            {
                // Raycast hits are sorted by distance, so the first one
                // will be the closest hit.
                var hitPose = s_Hits[0].pose;

                gameScene.transform.position = hitPose.position;
                raycastHeight = gameScene.transform.position.y;
                UpdatePlaneOffset();
            }
        }

        static List<ARRaycastHit> s_Hits = new List<ARRaycastHit>();

        ARRaycastManager m_RaycastManager;

        public void UpdatePlaneOffset()
        {
            gameScene.transform.position = new Vector3(gameScene.transform.position.x, raycastHeight, gameScene.transform.position.z);
            gameScene.transform.position += new Vector3(0, slider.value, 0);
        }
    }
}
