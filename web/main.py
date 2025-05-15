import cv2
import numpy as np
import urllib.request

class MonocularSLAM:
    def __init__(self):
        # Initialize feature detector with more points
        self.feature_params = dict(
            maxCorners=500,          # Increased from 100 to 500
            qualityLevel=0.1,        # Reduced from 0.3 to get more points
            minDistance=5,           # Reduced from 7 to get denser points
            blockSize=5              # Reduced from 7 for finer detection
        )
        
        # Initialize Lucas-Kanade optical flow parameters
        self.lk_params = dict(
            winSize=(11, 11),        # Reduced window size for better local tracking
            maxLevel=3,              # Increased from 2 for better tracking
            criteria=(cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 15, 0.01)
        )

        # Store previous frame and points
        self.prev_frame = None
        self.prev_points = None
        
        # Track age of each point
        self.point_ages = None
        self.max_age = 50  # Maximum frames to track a point
        
        # Frame counter
        self.frame_idx = 0
        
        # Color parameters
        self.base_colors = [
            (0, 255, 0),    # Green
            (255, 0, 0),    # Blue
            (0, 0, 255),    # Red
            (255, 255, 0),  # Cyan
            (0, 255, 255),  # Yellow
            (255, 0, 255)   # Magenta
        ]

    def process_frame(self, frame):
        if frame is None:
            return frame

        # Convert frame to grayscale
        frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        vis = frame.copy()

        if self.prev_frame is not None:
            # Track existing points
            if self.prev_points is not None and len(self.prev_points) > 0:
                curr_points, status, _ = cv2.calcOpticalFlowPyrLK(
                    self.prev_frame, frame_gray, self.prev_points, None, 
                    **self.lk_params
                )
                
                # Filter out points where tracking failed
                good = status.ravel() == 1
                curr_points = curr_points[good]
                prev_points = self.prev_points[good]
                if self.point_ages is not None:
                    self.point_ages = self.point_ages[good]
                    self.point_ages += 1
                
                # Update tracks with new points
                for idx, ((x, y), (px, py)) in enumerate(zip(curr_points.reshape(-1, 2), 
                                                           prev_points.reshape(-1, 2))):
                    # Color based on age
                    age = self.point_ages[idx] if self.point_ages is not None else 0
                    color_idx = min(int(age / 5), len(self.base_colors) - 1)
                    color = self.base_colors[color_idx]
                    
                    # Draw motion line
                    cv2.line(vis, (int(x), int(y)), (int(px), int(py)), color, 1)
                    
                    # Draw point with size based on age
                    pt_size = min(int(age/10) + 1, 4)
                    cv2.circle(vis, (int(x), int(y)), pt_size, color, -1)
                
                self.prev_points = curr_points.reshape(-1, 1, 2)
            
            # Detect new points periodically
            need_more = self.prev_points is None or len(self.prev_points) < 300
            if need_more and self.frame_idx % 3 == 0:  # Check more frequently
                mask = np.zeros_like(frame_gray)
                mask[:] = 255
                
                if self.prev_points is not None:
                    for x, y in self.prev_points.reshape(-1, 2):
                        cv2.circle(mask, (int(x), int(y)), 5, 0, -1)
                
                new_points = cv2.goodFeaturesToTrack(frame_gray, mask=mask,
                                                   **self.feature_params)
                
                if new_points is not None:
                    if self.prev_points is None:
                        self.prev_points = new_points
                        self.point_ages = np.zeros(len(new_points))
                    else:
                        self.prev_points = np.vstack([self.prev_points, new_points])
                        self.point_ages = np.hstack([self.point_ages, np.zeros(len(new_points))])
        
        else:
            # First frame - detect initial points
            self.prev_points = cv2.goodFeaturesToTrack(frame_gray, 
                                                      **self.feature_params)
            if self.prev_points is not None:
                self.point_ages = np.zeros(len(self.prev_points))
        
        # Draw stats
        if self.prev_points is not None:
            cv2.putText(vis, f'Tracked Points: {len(self.prev_points)}', 
                       (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            
            # Calculate average age of points
            avg_age = np.mean(self.point_ages) if self.point_ages is not None else 0
            cv2.putText(vis, f'Avg Age: {avg_age:.1f}', 
                       (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        
        # Update previous frame and counter
        self.prev_frame = frame_gray
        self.frame_idx += 1
        
        return vis

def stream_camera():
    stream_url = "http://192.168.137.163:80/capture"
    slam = MonocularSLAM()
    
    while True:
        try:
            # Read stream
            img_resp = urllib.request.urlopen(stream_url)
            imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
            frame = cv2.imdecode(imgnp, -1)
            frame = cv2.flip(frame, 0)
            
            
            if frame is not None:
                # Process frame with SLAM
                processed_frame = slam.process_frame(frame)
                cv2.imshow('Dense Feature Tracking', processed_frame)
                
                # Press 'q' to quit
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
                    
        except Exception as e:
            print(f"Error: {e}")
            continue
    
    cv2.destroyAllWindows()

if __name__ == '__main__':
    stream_camera()
