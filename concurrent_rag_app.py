from scheduler_bridge import SchedulerBridge

if __name__ == "__main__":
    dispatcher = SchedulerBridge()

    # Load multi-tier user pipeline queries simultaneously
    dispatcher.submit_query(priority_tier=3, payload_string="Query Batch C: Standard metrics scan request.")
    dispatcher.submit_query(priority_tier=1, payload_string="Query Batch A: CRITICAL system incident patch trace.")

    print("=== PRIORITYQUEUE-RAG RESOURCE BALANCING DEMO ===")
    
    # Process the high-priority task first, regardless of arrival order
    next_up = dispatcher.dispatch_highest_urgency()
    print(f"[*] Dispatched Task Target selected via Min-Heap Routing: {next_up}")
